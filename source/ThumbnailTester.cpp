#include "stdafx.h"
#include "ThumbnailTester.h"
#include <Spore\Cheats.h>
#include <Spore\GameModes.h>
#include <Spore\Editors\Editor.h>
#include <Spore\Graphics\IRenderer.h>
#include <Spore\Graphics\RenderUtils.h>
#include <d3dx9tex.h>

using namespace Editors;

ThumbnailTester::ThumbnailTester()
	: mpItemViewer(nullptr)
{
	MessageManager.AddListener(this, App::OnModeExitMessage::ID);
	App::AddUpdateFunction(this);
}


ThumbnailTester::~ThumbnailTester()
{
}

int ThumbnailTester::GetEventFlags() const {
	return UTFWin::kEventRefresh;
}

bool ThumbnailTester::HandleUIMessage(IWindow* pWindow, const Message& message) {
	if (message.IsType(kMsgMouseEnter)) {
		mpItemViewer = mItemViewers[pWindow];
	}
	else if (message.IsType(kMsgMouseLeave)) {
		mpItemViewer = nullptr;
	}
	return false;
}

float Normalize(float a, float b)
{
	float degs = a + ToDegrees(b);
	if (degs < -360.f) degs += 360.f;
	else if (degs > 360.f) degs -= 360.f;
	return ToRadians(degs);
}

#define SHIFTED 0x8000
void ThumbnailTester::Update() {
	if (mpItemViewer && mpItemViewer->mpModel)
	{
		Vector3 angles(0.f, 0.f, 0.f);
		if (GetKeyState('W') & SHIFTED) 
		{
			angles.y += 1.f;
		}
		if (GetKeyState('S') & SHIFTED) 
		{
			angles.y -= 1.f;
		}
		if (GetKeyState('A') & SHIFTED) 
		{
			angles.x += 1.f;
		}
		if (GetKeyState('D') & SHIFTED) 
		{
			angles.x -= 1.f;
		}
		if (GetKeyState('Q') & SHIFTED) 
		{
			angles.z += 1.f;
		}
		if (GetKeyState('E') & SHIFTED) 
		{
			angles.z -= 1.f;
		}

		if (angles != Vector3::ZERO)
		{
			const Vector3 current_angles = mpItemViewer->field_98.ToEuler();

			angles.x = Normalize(angles.x, current_angles.x);
			angles.y = Normalize(angles.y, current_angles.y);
			angles.z = Normalize(angles.z, current_angles.z);

			mpItemViewer->field_98 = Matrix3::FromEuler(angles);
			mpItemViewer->mRotation = 0;
			mpItemViewer->field_6C = 0;
			mpItemViewer->RotateModel();
		}
	}
}

void ThumbnailTester::InjectListeners() {
	RemoveListeners();
	for (auto catUI : GetEditor()->mpPartsPaletteUI->mCategories) {
		for (auto pageUI : catUI->mPageUIs) {
			for (auto itemUI : pageUI.page->mStandardItems) {
				itemUI->field_18->AddWinProc(this);
				mItemViewers[itemUI->field_18.get()] = itemUI->mpViewer.get();
			}
		}
	}
}

void ThumbnailTester::RemoveListeners() {
	mItemViewers.clear();
}

void ThumbnailTester::ParseLine(const ArgScript::Line& line) {
	if (!Editor.IsActive()) {
		App::ConsolePrintF("Must be in editor to use this cheat.");
		return;
	}
	if (!Editor.IsMode(Mode::BuildMode)) {
		App::ConsolePrintF("Must be in editor build mode to use this cheat.");
		return;
	}

	auto args = line.GetOption("enable", 1);
	if (args)
	{
		if (mpFormatParser->ParseBool(args[0]))
			InjectListeners();
		else
			RemoveListeners();
		return;
	}


	if (!mpItemViewer || !mpItemViewer->mpModel) {
		App::ConsolePrintF("Must be hovering over a part in the editor to use this cheat.");
		return;
	}
	const Vector3 ua = mpItemViewer->field_98.ToEuler();
	App::ConsolePrintF("vector3 modelSwatchRotation (%d, %d, %d)", static_cast<int>(ToDegrees(ua.x)), static_cast<int>(ToDegrees(ua.y)), static_cast<int>(ToDegrees(ua.z)));
}

int ThumbnailTester::AddRef() {
	return DefaultObject::AddRef();
}
int ThumbnailTester::Release() {
	return DefaultObject::Release();
}
void* ThumbnailTester::Cast(uint32_t type) const {
	if (type == IWinProc::TYPE) return (IWinProc*)this;
	else return DefaultObject::Cast(type);
}

void ThumbnailTester::AddCheat() {
	CheatManager.AddCheat("modelSwatchRotation", new ThumbnailTester());
}

bool ThumbnailTester::HandleMessage(uint32_t messageID, void* message) {
	if (messageID == App::kMsgOnModeExit) {
		auto data = (App::OnModeExitMessage*)message;
		if (data->GetPreviousModeID() == GameModeIDs::kEditorMode) {
			RemoveListeners();
		}
	}
	return false;
}
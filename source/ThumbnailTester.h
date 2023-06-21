#pragma once

#include <Spore\UTFWin\IWinProc.h>
#include <Spore\Cheats.h>
#include <Spore\Palettes\ItemViewer.h>
#include <EASTL\hash_map.h>

using namespace UTFWin;

class ThumbnailTester
	: public IWinProc
	, public ArgScript::ICommand
	, public DefaultObject
	, public App::IMessageListener
	, public App::IUpdatable
{
public:
	ThumbnailTester();
	virtual ~ThumbnailTester();

	int AddRef() override;
	int Release() override;
	void* Cast(uint32_t) const override;
	
	int GetEventFlags() const override;
	bool HandleUIMessage(IWindow* pWindow, const Message& message) override;

	void ParseLine(const ArgScript::Line& line) override;

	bool HandleMessage(uint32_t messageID, void* pMessage) override;

	void Update();

	static void AddCheat();

private:
	void InjectListeners();
	void RemoveListeners();

	hash_map<IWindow*, Palettes::ItemViewer*> mItemViewers;

	Palettes::ItemViewer* mpItemViewer;
};


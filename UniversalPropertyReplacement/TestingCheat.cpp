/****************************************************************************
* Copyright (C) 2018 Zarklord
*
* This file is part of UniversalPropertyReplacement.
*
* UniversalPropertyReplacement is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with UniversalPropertyReplacement.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "stdafx.h"
#include "TestingCheat.h"
#include <Spore\Hash.h>
#include <Spore\App\IPropManager.h>
#include <Spore\App\PropertyList.h>
#include <Spore\Cheats.h>
#include <cwchar>

void TestingCheat::ParseLine(const ArgScript::Line & line) {
	App::PropertyList::Pointer pProp = nullptr;
	App::IPropManager::Get()->GetPropertyList(Hash::FNV("testing"), Hash::FNV("testing"), pProp);
	bool testFailed = false;
	bool boolTest;
	App::Property::GetBool(pProp.get(), Hash::FNV("boolValue"), boolTest);
	if (boolTest == false) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "boolReplacement failed!");
		testFailed = true;
	}
	float floatTest;
	App::Property::GetFloat(pProp.get(), Hash::FNV("floatValue"), floatTest);
	if (floatTest == 2.0) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "floatReplacement failed!");
		testFailed = true;
	}
	int32_t int32Test;
	App::Property::GetInt32(pProp.get(), Hash::FNV("intValue"), int32Test);
	if (int32Test == -10) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "int32Replacement failed!");
		testFailed = true;
	}
	uint32_t uint32Test;
	App::Property::GetUInt32(pProp.get(), Hash::FNV("uintValue"), uint32Test);
	if (uint32Test == 65535) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "uint32Replacement failed!");
		testFailed = true;
	}
	Vector2 vector2Test;
	App::Property::GetVector2(pProp.get(), Hash::FNV("vector2Value"), vector2Test);
	if (vector2Test.x == 13 && vector2Test.y == 37) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "vector2Replacement failed!");
		testFailed = true;
	}
	Vector3 vector3Test;
	App::Property::GetVector3(pProp.get(), Hash::FNV("vector3Value"), vector3Test);
	if (vector3Test.x == 1 && vector3Test.y == 33 && vector3Test.z == 7) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "vector3Replacement failed!");
		testFailed = true;
	}
	Vector4 vector4Test;
	App::Property::GetVector4(pProp.get(), Hash::FNV("vector4Value"), vector4Test);
	if (vector4Test.x == 1 && vector4Test.y == 3 && vector4Test.z == 3 && vector4Test.w == 7) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "vector4Replacement failed!");
		testFailed = true;
	}
	ColorRGB colorRGBTest;
	App::Property::GetColorRGB(pProp.get(), Hash::FNV("colorRGBValue"), colorRGBTest);
	if (colorRGBTest.r == 0.0 && colorRGBTest.g == 1.0 && colorRGBTest.b == 0.0) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "colorRGBReplacement failed!");
		testFailed = true;
	}
	ColorRGBA colorRGBATest;
	App::Property::GetColorRGBA(pProp.get(), Hash::FNV("colorRGBAValue"), colorRGBATest);
	if (colorRGBATest.r == 0 && colorRGBATest.g == 1 && colorRGBATest.b == 0 && colorRGBATest.a == 0.5) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "colorRGBAReplacement failed!");
		testFailed = true;
	}
	ResourceKey keyTest;
	App::Property::GetKey(pProp.get(), Hash::FNV("keyoverrideValue"), keyTest);
	if (keyTest.mnGroupID == Hash::FNV("prop_overrides") && keyTest.mnInstanceID == Hash::FNV("testingOverrides") && keyTest.mnTypeID == Hash::FNV("prop")) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "keyReplacement failed!");
		testFailed = true;
	}
	eastl::string stringTest;
	App::Property::GetString8(pProp.get(), Hash::FNV("string8Value"), stringTest);
	if (stringTest == "string8") {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "string8Replacement failed!");
		testFailed = true;
	}
	char * cstringTest;
	App::Property::GetCString8(pProp.get(), Hash::FNV("cstring8Value"), cstringTest);
	if (strcmp(cstringTest, "cstring8") == 0) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "cstring8Replacement failed!");
		testFailed = true;
	}
	eastl::string16 string16Test;
	App::Property::GetString16(pProp.get(), Hash::FNV("string16Value"), string16Test);
	if (string16Test.compare(eastl::string16{"string16"}) == 0) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "string16Replacement failed!");
		testFailed = true;
	}
	wchar_t * wcstringTest;
	App::Property::GetCString16(pProp.get(), Hash::FNV("cstring16Value"), wcstringTest);
	if (wcscmp(wcstringTest, L"cstring16") == 0) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "cstring16Replacement failed!");
		testFailed = true;
	}

	if (!testFailed) {
		ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "no replacement tests failed!");
	}
}

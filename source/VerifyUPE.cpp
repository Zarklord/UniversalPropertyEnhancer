/****************************************************************************
* Copyright (C) 2018, 2020, 2022 Zarklord
*
* This file is part of UniversalPropertyEnhancer.
*
* UniversalPropertyEnhancer is free software: you can redistribute it and/or modify
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
* along with UniversalPropertyEnhancer.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

#include "stdafx.h"
#include "VerifyUPE.h"

#include "PropertyExt.h"
#include "typename.h"
#include "UniversalProperty/Replacement.h"

bool VerifyValue(const bool& toverify) {
	return toverify == false;
}
bool VerifyValue(const int32_t& toverify) {
	return toverify == -1;
}
bool VerifyValue(const  uint32_t& toverify) {
	return toverify == 1;
}
bool VerifyValue(const float& toverify) {
	return toverify == 1.1f;
}
bool VerifyValue(const eastl::string& toverify) {
	return toverify == "1";
}
bool VerifyValue(const char*& toverify) {
	return strcmp(toverify, "1") == 0;
}
bool VerifyValue(const eastl::string16& toverify) {
	return toverify == eastl::string16 {"1"};
}
bool VerifyValue(const char16_t*& toverify) {
	return wcscmp(reinterpret_cast<const wchar_t*>(toverify), L"1") == 0;
}
bool VerifyValue(const ResourceKey& toverify) {
	return toverify.groupID == id("prop_overrides") && toverify.instanceID == id("VerifyUPEOverrides") && toverify.typeID == id("prop");
}
bool VerifyValue(const LocalizedString& toverify) {
	return toverify.GetText() == eastl::string16 {"Name Your Creature"};
}
bool VerifyValue(const Vector2& toverify) {
	return toverify.x == 1.f && toverify.y == 1.f;
}
bool VerifyValue(const Vector3& toverify) {
	return toverify.x == 1.f && toverify.y == 1.f && toverify.z == 1.f;
}
bool VerifyValue(const Vector4& toverify) {
	return toverify.x == 1.f && toverify.y == 1.f && toverify.z == 1.f && toverify.w == 1.f;
}
bool VerifyValue(const ColorRGB& toverify) {
	return toverify.r == 1.f && toverify.g == 1.f && toverify.b == 1.f;
}
bool VerifyValue(const ColorRGBA& toverify) {
	return toverify.r == 1.f && toverify.g == 1.f && toverify.b == 1.f && toverify.a == 1.f;
}
bool VerifyValue(const Transform& toverify) {
	const auto o = toverify.GetOffset();
	const auto s = toverify.GetScale();
	const auto r = toverify.GetRotation().ToEuler();
	return o.x== 1.f && o.y== 1.f && o.z== 1.f && s== 1.f && r.x== 1.f && r.y== 1.f && r.z== 1.f;
}
bool VerifyValue(const BoundingBox& toverify) {
	const auto l = toverify.lower;
	const auto u = toverify.upper;
	return l.x== 1.f && l.y== 1.f && l.z== 1.f && u.x== 1.f && u.y== 1.f && u.z== 1.f;
}

bool VerifyCount(const uint32_t& count) {
	return count == 3;
}

void ReportReplacementFailed(const eastl::string& str) {
	App::ConsolePrintF("%s Replacement failed!", str.c_str());
}

uint32_t GetPropertyId(const eastl::string& str) {
	return id((str + "Value").c_str());
}

void VerifyUPE::ParseLine(const ArgScript::Line& line) {
	string error_string;
	const bool upr_results = GetUniversalPropertyReplacer().GetTestResults(error_string);
	App::ConsolePrintF("UniversalPropertyReplacer Test Results -> %s", upr_results ? "Passed" : "Failed");
	if (!upr_results)
		App::ConsolePrintF("UniversalPropertyReplacer Errors:\n%s", error_string);

	PropertyListPtr propList;
	PropManager.GetPropertyList(id("VerifyUPE"), id("VerifyUPE"), propList);

	bool testFailed = false;

	bool addTest = false;
	App::Property::GetBool(propList.get(), id("postinitAdd"), addTest);
	if (VerifyValue(addTest)) {
		App::ConsolePrintF("Add Postinit failed!");
		testFailed = true;
	}
	bool removeTest = false;
	if (App::Property::GetBool(propList.get(), id("postinitRemove"), removeTest)) {
		App::ConsolePrintF("Remove Postinit failed!");
		testFailed = true;
	}
	int32_t replaceTest;
	App::Property::GetInt32(propList.get(), id("postinitReplace"), replaceTest);
	if (VerifyValue(replaceTest)) {
		App::ConsolePrintF("Replace Postinit failed!");
		testFailed = true;
	}
	uint32_t remainTest;
	App::Property::GetUInt32(propList.get(), id("postinitRemain"), remainTest);
	if (VerifyValue(remainTest)) {
		App::ConsolePrintF("Remain Postinit failed!");
		testFailed = true;
	}

	if (!testFailed) {
		App::ConsolePrintF("All postinit tests passed!");
	}
}
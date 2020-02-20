/****************************************************************************
* Copyright (C) 2018, 2020 Zarklord
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
#include "VerificationCheat.h"

bool VerifyValue(bool& toverify) {
	return toverify == false;
}
bool VerifyValue(int32_t& toverify) {
	return toverify == -1;
}
bool VerifyValue(uint32_t& toverify) {
	return toverify == 1;
}
bool VerifyValue(float& toverify) {
	return toverify == 1.1;
}
bool VerifyValue(eastl::string& toverify) {
	return toverify == "1";
}
bool VerifyValue(char*& toverify) {
	return strcmp(toverify, "1") == 0;
}
bool VerifyValue(eastl::string16& toverify) {
	return toverify == eastl::string16 {"1"};
}
bool VerifyValue(char16_t*& toverify) {
	return wcscmp((wchar_t*)toverify, L"1") == 0;
}
bool VerifyValue(ResourceKey& toverify) {
	return toverify.groupID == id("prop_overrides") && toverify.instanceID == id("VerifyUPEOverrides") && toverify.typeID == id("prop");
}
bool VerifyValue(LocalizedString& toverify) {
	return toverify.GetText() == eastl::string16 {"Name Your Creature"};
}
bool VerifyValue(Vector2& toverify) {
	return toverify.x == 1 && toverify.y == 1;
}
bool VerifyValue(Vector3& toverify) {
	return toverify.x == 1 && toverify.y == 1 && toverify.z == 1;
}
bool VerifyValue(Vector4& toverify) {
	return toverify.x == 1 && toverify.y == 1 && toverify.z == 1 && toverify.w == 1;
}
bool VerifyValue(ColorRGB& toverify) {
	return toverify.r == 1 && toverify.g == 1 && toverify.b == 1;
}
bool VerifyValue(ColorRGBA& toverify) {
	return toverify.r == 1 && toverify.g == 1 && toverify.b == 1 && toverify.a == 1;
}
bool VerifyValue(Transform& toverify) {
	auto o = toverify.GetOffset();
	auto s = toverify.GetScale();
	auto r = toverify.GetRotation().ToEuler();
	return o.x == 1 && o.y == 1 && o.z == 1 && s == 1 && r.x == 1 && r.y == 1 && r.z == 1;
}
bool VerifyValue(BoundingBox& toverify) {
	auto l = toverify.lower;
	auto u = toverify.upper;
	return l.x == 1 && l.y == 1 && l.z == 1 && u.x == 1 && u.y == 1 && u.z == 1;
}

bool VerifyCount(uint32_t& count) {
	return count == 3;
}

void ReportReplacementFailed(eastl::string str) {
	App::ConsolePrintF("%s Replacement failed!", str.c_str());
}

uint32_t GetPropertyId(eastl::string str) {
	return id((str + "Value").c_str());
}

void VerificationCheat::ParseLine(const ArgScript::Line& line) {
	PropertyListPtr propList;
	PropManager.GetPropertyList(id("VerifyUPE"), id("VerifyUPE"), propList);

	App::Property* out;
	bool testFailed = false;

	bool deleteTest;
	if (App::Property::GetBool(propList.get(), GetPropertyId("delete"), deleteTest)) {
		ReportReplacementFailed("delete");
		testFailed = true;
	}
	bool boolTest = false;
	App::Property::GetBool(propList.get(), GetPropertyId("bool"), boolTest);
	if (VerifyValue(boolTest)) {
		ReportReplacementFailed("bool");
		testFailed = true;
	}
	int32_t int32Test;
	App::Property::GetInt32(propList.get(), GetPropertyId("int32"), int32Test);
	if (VerifyValue(int32Test)) {
		ReportReplacementFailed("int32");
		testFailed = true;
	}
	uint32_t uint32Test;
	App::Property::GetUInt32(propList.get(), GetPropertyId("uint32"), uint32Test);
	if (VerifyValue(uint32Test)) {
		ReportReplacementFailed("uint32");
		testFailed = true;
	}
	float floatTest;
	App::Property::GetFloat(propList.get(), GetPropertyId("float"), floatTest);
	if (VerifyValue(floatTest)) {
		ReportReplacementFailed("float");
		testFailed = true;
	}
	eastl::string stringTest;
	App::Property::GetString8(propList.get(), GetPropertyId("string8"), stringTest);
	if (VerifyValue(stringTest)) {
		ReportReplacementFailed("string8");
		testFailed = true;
	}
	char* cstringTest;
	App::Property::GetCString8(propList.get(), GetPropertyId("string8"), cstringTest);
	if (VerifyValue(cstringTest)) {
		ReportReplacementFailed("cstring8");
		testFailed = true;
	}
	eastl::string16 string16Test;
	App::Property::GetString16(propList.get(), GetPropertyId("string16"), string16Test);
	if (VerifyValue(string16Test)) {
		ReportReplacementFailed("string16");
		testFailed = true;
	}
	char16_t* cstring16Test;
	App::Property::GetCString16(propList.get(), GetPropertyId("string16"), cstring16Test);
	if (VerifyValue(cstring16Test)) {
		ReportReplacementFailed("cstring16");
		testFailed = true;
	}
	ResourceKey keyTest;
	App::Property::GetKey(propList.get(), GetPropertyId("key"), keyTest);
	if (VerifyValue(keyTest)) {
		ReportReplacementFailed("key");
		testFailed = true;
	}
	LocalizedString textTest;
	App::Property::GetText(propList.get(), GetPropertyId("text"), textTest);
	if (VerifyValue(textTest)) {
		ReportReplacementFailed("text");
		testFailed = true;
	}
	Vector2 vector2Test;
	App::Property::GetVector2(propList.get(), GetPropertyId("vector2"), vector2Test);
	if (VerifyValue(vector2Test)) {
		ReportReplacementFailed("vector2");
		testFailed = true;
	}
	Vector3 vector3Test;
	App::Property::GetVector3(propList.get(), GetPropertyId("vector3"), vector3Test);
	if (VerifyValue(vector3Test)) {
		ReportReplacementFailed("vector3");
		testFailed = true;
	}
	Vector4 vector4Test;
	App::Property::GetVector4(propList.get(), GetPropertyId("vector4"), vector4Test);
	if (VerifyValue(vector4Test)) {
		ReportReplacementFailed("vector4");
		testFailed = true;
	}
	ColorRGB colorRGBTest;
	App::Property::GetColorRGB(propList.get(), GetPropertyId("colorRGB"), colorRGBTest);
	if (VerifyValue(colorRGBTest)) {
		ReportReplacementFailed("colorRGB");
		testFailed = true;
	}
	ColorRGBA colorRGBATest;
	App::Property::GetColorRGBA(propList.get(), GetPropertyId("colorRGBA"), colorRGBATest);
	if (VerifyValue(colorRGBATest)) {
		ReportReplacementFailed("colorRGBA");
		testFailed = true;
	}
	Transform transformTest;
	if (propList->GetProperty(GetPropertyId("transform"), out)) {
		transformTest = *(out->GetValueTransform());
		if (VerifyValue(transformTest)) {
			ReportReplacementFailed("transform");
			testFailed = true;
		}
	}
	BoundingBox bBoxTest;
	if (propList->GetProperty(GetPropertyId("bbox"), out)) {
		bBoxTest = *(out->GetValueBBox());
		if (VerifyValue(bBoxTest)) {
			ReportReplacementFailed("bbox");
			testFailed = true;
		}
	}

	uint32_t count;

	bool* boolsTest;
	if (propList->GetProperty(GetPropertyId("bools"), out)) {
		boolsTest = out->GetValueBool();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(boolsTest[0]) && VerifyValue(boolsTest[1]))) {
			ReportReplacementFailed("bools");
			testFailed = true;
		}
	}
	int32_t* int32sTest;
	if (propList->GetProperty(GetPropertyId("int32s"), out)) {
		int32sTest = out->GetValueInt32();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(int32sTest[0]) && VerifyValue(int32sTest[1]))) {
			ReportReplacementFailed("int32s");
			testFailed = true;
		}
	}
	uint32_t* uint32sTest;
	if (propList->GetProperty(GetPropertyId("uint32s"), out)) {
		uint32sTest = out->GetValueUInt32();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(uint32sTest[0]) && VerifyValue(uint32sTest[1]))) {
			ReportReplacementFailed("uint32s");
			testFailed = true;
		}
	}
	float* floatsTest;
	if (propList->GetProperty(GetPropertyId("floats"), out)) {
		floatsTest = out->GetValueFloat();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(floatsTest[0]) && VerifyValue(floatsTest[1]))) {
			ReportReplacementFailed("floats");
			testFailed = true;
		}
	}
	eastl::string* string8sTest;
	if (propList->GetProperty(GetPropertyId("string8s"), out)) {
		string8sTest = out->GetValueString8();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(string8sTest[0]) && VerifyValue(string8sTest[1]))) {
			ReportReplacementFailed("string8s");
			testFailed = true;
		}
	}
	eastl::string16* string16sTest;
	if (propList->GetProperty(GetPropertyId("string16s"), out)) {
		string16sTest = out->GetValueString16();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(string16sTest[0]) && VerifyValue(string16sTest[1]))) {
			ReportReplacementFailed("string16s");
			testFailed = true;
		}
	}
	ResourceKey* keysTest;
	if (propList->GetProperty(GetPropertyId("keys"), out)) {
		keysTest = out->GetValueKey();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(keysTest[0]) && VerifyValue(keysTest[1]))) {
			ReportReplacementFailed("keys");
			testFailed = true;
		}
	}
	LocalizedString* textsTest;
	if (propList->GetProperty(GetPropertyId("texts"), out)) {
		textsTest = out->GetValueText();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(textsTest[0]) && VerifyValue(textsTest[1]))) {
			ReportReplacementFailed("texts");
			testFailed = true;
		}
	}
	Vector2* vector2sTest;
	if (propList->GetProperty(GetPropertyId("vector2s"), out)) {
		vector2sTest = out->GetValueVector2();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(vector2sTest[0]) && VerifyValue(vector2sTest[1]))) {
			ReportReplacementFailed("vector2s");
			testFailed = true;
		}
	}
	Vector3* vector3sTest;
	if (propList->GetProperty(GetPropertyId("vector3s"), out)) {
		vector3sTest = out->GetValueVector3();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(vector3sTest[0]) && VerifyValue(vector3sTest[1]))) {
			ReportReplacementFailed("vector3s");
			testFailed = true;
		}
	}
	Vector4* vector4sTest;
	if (propList->GetProperty(GetPropertyId("vector4s"), out)) {
		vector4sTest = out->GetValueVector4();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(vector4sTest[0]) && VerifyValue(vector4sTest[1]))) {
			ReportReplacementFailed("vector4s");
			testFailed = true;
		}
	}
	ColorRGB* colorRGBsTest;
	if (propList->GetProperty(GetPropertyId("colorRGBs"), out)) {
		colorRGBsTest = out->GetValueColorRGB();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(colorRGBsTest[0]) && VerifyValue(colorRGBsTest[1]))) {
			ReportReplacementFailed("colorRGBs");
			testFailed = true;
		}
	}
	ColorRGBA* colorRGBAsTest;
	if (propList->GetProperty(GetPropertyId("colorRGBAs"), out)) {
		colorRGBAsTest = out->GetValueColorRGBA();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(colorRGBAsTest[0]) && VerifyValue(colorRGBAsTest[1]))) {
			ReportReplacementFailed("colorRGBAs");
			testFailed = true;
		}
	}
	Transform* transformsTest;
	if (propList->GetProperty(GetPropertyId("transforms"), out)) {
		transformsTest = out->GetValueTransform();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(transformsTest[0]) && VerifyValue(transformsTest[1]))) {
			ReportReplacementFailed("transforms");
			testFailed = true;
		}
	}
	BoundingBox* bBoxsTest;
	if (propList->GetProperty(GetPropertyId("bboxs"), out)) {
		bBoxsTest = out->GetValueBBox();
		count = out->GetItemCount();
		if (VerifyCount(count) || (VerifyValue(bBoxsTest[0]) && VerifyValue(bBoxsTest[1]))) {
			ReportReplacementFailed("bboxs");
			testFailed = true;
		}
	}

	if (!testFailed) {
		App::ConsolePrintF("All replacement tests passed!");
	}

	testFailed = false;

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
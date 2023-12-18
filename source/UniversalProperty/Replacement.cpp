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
#include "Replacement.h"

UniversalPropertyReplacer& GetUniversalPropertyReplacer()
{
	return UniversalPropertyReplacer::Get();
}

UniversalPropertyReplacer* UniversalPropertyReplacer::mInstance = nullptr;
void UniversalPropertyReplacer::Initialize()
{
	mInstance = new UniversalPropertyReplacer();
	mInstance->PostConstruct();
}

void UniversalPropertyReplacer::Finalize()
{
	delete mInstance;
	mInstance = nullptr;
}

UniversalPropertyReplacer& UniversalPropertyReplacer::Get()
{
	return *mInstance;
}

bool UniversalPropertyReplacer::Exists()
{
	return mInstance != nullptr;
}


using PropertyType = App::PropertyType;
UniversalPropertyReplacer::UniversalPropertyReplacer()
{
	mValueReplacements.insert(new ReplaceType<bool, PropertyType::Bool, false, id("bool")>());
	mArrayReplacements.insert(new ReplaceType<bool, PropertyType::Bool, true, id("bools")>());

	mValueReplacements.insert(new ReplaceType<int32_t, PropertyType::Int32, false, id("int32")>());
	mArrayReplacements.insert(new ReplaceType<int32_t, PropertyType::Int32, true, id("int32s")>());

	mValueReplacements.insert(new ReplaceType<uint32_t, PropertyType::UInt32, false, id("uint32")>());
	mArrayReplacements.insert(new ReplaceType<uint32_t, PropertyType::UInt32, true, id("uint32s")>());

	mValueReplacements.insert(new ReplaceType<float, PropertyType::Float, false, id("float")>());
	mArrayReplacements.insert(new ReplaceType<float, PropertyType::Float, true, id("floats")>());

	mValueReplacements.insert(new ReplaceType<string8, PropertyType::String8, false, id("string8")>());
	mArrayReplacements.insert(new ReplaceType<string8, PropertyType::String8, true, id("string8s")>());

	mValueReplacements.insert(new ReplaceType<string16, PropertyType::String16, false, id("string16")>());
	mArrayReplacements.insert(new ReplaceType<string16, PropertyType::String16, true, id("string16s")>());

	mValueReplacements.insert(new ReplaceType<ResourceKey, PropertyType::Key, false, id("key")>());
	mArrayReplacements.insert(new ReplaceType<ResourceKey, PropertyType::Key, true, id("keys")>());

	mValueReplacements.insert(new ReplaceType<LocalizedString, PropertyType::Text, false, id("text")>());
	mArrayReplacements.insert(new ReplaceType<LocalizedString, PropertyType::Text, true, id("texts")>());

	mValueReplacements.insert(new ReplaceType<Vector2, PropertyType::Vector2, false, id("vector2")>());
	mArrayReplacements.insert(new ReplaceType<Vector2, PropertyType::Vector2, true, id("vector2s")>());

	mValueReplacements.insert(new ReplaceType<Vector3, PropertyType::Vector3, false, id("vector3")>());
	mArrayReplacements.insert(new ReplaceType<Vector3, PropertyType::Vector3, true, id("vector3s")>());

	mValueReplacements.insert(new ReplaceType<Vector4, PropertyType::Vector4, false, id("vector4")>());
	mArrayReplacements.insert(new ReplaceType<Vector4, PropertyType::Vector4, true, id("vector4s")>());

	mValueReplacements.insert(new ReplaceType<ColorRGB, PropertyType::ColorRGB, false, id("colorrgb")>());
	mArrayReplacements.insert(new ReplaceType<ColorRGB, PropertyType::ColorRGB, true, id("colorrgbs")>());

	mValueReplacements.insert(new ReplaceType<ColorRGBA, PropertyType::ColorRGBA, false, id("colorrgba")>());
	mArrayReplacements.insert(new ReplaceType<ColorRGBA, PropertyType::ColorRGBA, true, id("colorrgbas")>());
	
	mArrayReplacements.insert(new ReplaceType<Transform, PropertyType::Transform, true, id("transforms"), id("transform")>());
	
	mArrayReplacements.insert(new ReplaceType<BoundingBox, PropertyType::BBox, true, id("bboxs"), id("bbox")>());

	LoadReplacementLists();
}

void UniversalPropertyReplacer::PostConstruct()
{
	Test();
}

UniversalPropertyReplacer::~UniversalPropertyReplacer()
{
	for (const auto replacement : mValueReplacements)
		delete replacement;

	for (const auto replacement : mArrayReplacements)
		delete replacement;
}

void UniversalPropertyReplacer::ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID)
{
	const Extensions::Property* ext_prop = GetPropertyExt(prop);
	if (mDeletePropertyIDs.find(propertyID) != mDeletePropertyIDs.end()) {
		prop->Set(App::PropertyType::Void, 0, nullptr, 0, 0);
		return;
	}

	if (ext_prop->IsArray())
	{
		const auto replace = mArrayReplacements.find_as(prop->mnType);
		if (replace != mArrayReplacements.end())
		{
			(*replace)->ApplyValueMapProperty(prop, propertyID);
		}
	}
	else
	{
		const auto replace = mValueReplacements.find_as(prop->mnType);
		if (replace != mValueReplacements.end())
		{
			(*replace)->ApplyValueMapProperty(prop, propertyID);
		}
	}
}

void UniversalPropertyReplacer::LoadReplacementLists()
{
	vector<uint32_t> instanceList{};
	PropManager.GetPropertyListIDs(sGroupID, instanceList);

	for (const auto instance : instanceList) {
		PropertyListPtr propList;
		PropManager.GetPropertyList(instance, sGroupID, propList);
		
		size_t stringCount = 0;
		Extensions::Property::array_string8 stringList;
		Extensions::Property::GetArrayString8(propList.get(), sArgumentList, stringCount, stringList);
		for (size_t j = 0; j < stringCount; j++) {
			ArgScript::Line PropLine = ArgScript::Line(stringList[j]);
			if (PropLine.GetArgumentsCount() > 2 && PropLine.GetArgumentsCount() > 4) continue;
			const uint32_t typeHash = id(PropLine.GetArgumentAt(0));

			eastl::string replace_str = PropLine.GetArgumentAt(1);
			const uint32_t replaceHash = std::strtoul(replace_str.c_str(), nullptr, 16);
			replace_str.pop_back();
			const uint32_t altReplaceHash = std::strtoul(replace_str.c_str(), nullptr, 16);
			if (typeHash == sDelete || altReplaceHash == sDelete) {
				mDeletePropertyIDs.insert(replaceHash);
				continue;
			}

			eastl::string8 value_str = PropLine.GetArgumentAt(2);
			const uint32_t valueHash = id(value_str.c_str());
			value_str.pop_back();
			const uint32_t altValueHash = id(value_str.c_str());

			bool finished = false;
			for (const auto replacement : mValueReplacements)
			{
				finished = replacement->LoadValueMapProperty(typeHash, propList.get(), valueHash, replaceHash);
				if (finished) break;
				finished = replacement->LoadValueMapProperty(typeHash, propList.get(), altValueHash, replaceHash);
				if (finished) break;
			}
			if (finished) continue;

			for (const auto replacement : mArrayReplacements)
			{
				finished = replacement->LoadValueMapProperty(typeHash, propList.get(), valueHash, replaceHash);
				if (finished) break;
				finished = replacement->LoadValueMapProperty(typeHash, propList.get(), altValueHash, replaceHash);
				if (finished) break;
			}
		}
	}
}

bool UniversalPropertyReplacer::GetTestResults(string& error_string) const
{
	error_string = mErrorString;
	return mErrorString.empty();
}

void UniversalPropertyReplacer::Test()
{
	const auto temp_list = std::make_unique<App::PropertyList>();
	
	App::Property prop{};
	bool test_property = true;
	prop.Set(App::PropertyType::Bool, Extensions::Property::kPropertyFlagNone, &test_property, sizeof(bool), 1);
	temp_list->SetProperty(sUniversalPropertyReplacerTest, &prop);
	mDeletePropertyIDs.emplace(sUniversalPropertyReplacerTest);
	
	App::Property* get_prop{};
	temp_list->GetProperty(sUniversalPropertyReplacerTest, get_prop);
	const App::Property *get_prop_object = temp_list->GetPropertyObject(sUniversalPropertyReplacerTest);

	if (get_prop->mnType == App::PropertyType::Bool || get_prop_object->mnType == App::PropertyType::Bool)
	{
		const eastl::string failure = "Failed property deletion";
		ModAPI::Log(failure.c_str());
		mErrorString += failure + "\n";
	}
	mDeletePropertyIDs.erase(sUniversalPropertyReplacerTest);
	temp_list->RemoveProperty(sUniversalPropertyReplacerTest);


	for (const auto replacement : mValueReplacements)
	{
		replacement->Test(temp_list.get(), mErrorString);
	}

	for (const auto replacement : mArrayReplacements)
	{
		replacement->Test(temp_list.get(), mErrorString);
	}
	ModAPI::Log("UniversalPropertyReplacer Test Results -> %s", mErrorString.empty() ? "Passed" : "Failed");
}

member_detour(GetPropertyAlt_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result) {
		return this->GetProperty(propertyID, result);
	}
};

member_detour(GetProperty_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result) {
		const bool ret = original_function(this, propertyID, result);
		if (ret && UniversalPropertyReplacer::Exists()) GetUniversalPropertyReplacer().ApplyValueMapProperty(result, propertyID);
		return ret;	
	}
};

member_detour(GetPropertyObject_detour, App::PropertyList, App::Property*(uint32_t propertyID))
{
	App::Property* detoured(uint32_t propertyID) {
		App::Property* ret = original_function(this, propertyID);
		if (UniversalPropertyReplacer::Exists()) GetUniversalPropertyReplacer().ApplyValueMapProperty(ret, propertyID);
		return ret;
	}
};

void UniversalPropertyReplacer::AttachDetours() {
	GetPropertyAlt_detour::attach(GetAddress(App::PropertyList, GetPropertyAlt));
	GetProperty_detour::attach(GetAddress(App::PropertyList, GetProperty));
	GetPropertyObject_detour::attach(GetAddress(App::PropertyList, GetPropertyObject));
}
/****************************************************************************
* Copyright (C) 2020, 2022 Zarklord
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
#include "PostInit.h"

#include "Duplicator.h"
#include "PropertyExt.h"

PropertyListPostInitializer& GetPropertyListPostInitializer()
{
	return PropertyListPostInitializer::Get();
}

PropertyListPostInitializer* PropertyListPostInitializer::mInstance = nullptr;
void PropertyListPostInitializer::Initialize()
{
	mInstance = new PropertyListPostInitializer();
	mInstance->PostConstruct();
}

void PropertyListPostInitializer::Finalize()
{
	delete mInstance;
	mInstance = nullptr;
}

PropertyListPostInitializer& PropertyListPostInitializer::Get()
{
	return *mInstance;
}

bool PropertyListPostInitializer::Exists()
{
	return mInstance != nullptr;
}

PropertyListPostInitializer::PropertyListPostInitializer()
{
	LoadPostInitList();
}

void PropertyListPostInitializer::PostConstruct()
{
	Test();
}

PropertyListPostInitializer::~PropertyListPostInitializer()
{
	mPostInits.clear();
}

void PropertyListPostInitializer::ApplyPostInits(GetPropertyList fn, App::IPropManager* this_ptr, uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst)
{
	for (const auto i : mPostInits) {
		if (i.first.groupID == groupID && i.first.instanceID == instanceID) {
			PropertyListPtr postProp;
			if (!fn(this_ptr, i.second.instanceID, i.second.groupID, postProp)) continue;
			
			if (postProp->HasProperty(sNewParent)) {
				ResourceKey newParentKey;
				App::Property::GetKey(postProp.get(), sNewParent, newParentKey);

				PropertyListPtr newParentProp;
				PropManager.GetPropertyList(newParentKey.instanceID, newParentKey.groupID, newParentProp);

				pDst->SetParent(newParentProp.get());
				postProp->RemoveProperty(sNewParent);
			}
			
			if (postProp->HasProperty(sRemoveProperties)) {
				uint32_t* properties;
				size_t count;
				if (App::Property::GetArrayUInt32(postProp.get(), sRemoveProperties, count, properties)) {
					for (size_t j = 0; j < count; j++) {
						pDst->RemoveProperty(properties[j]);
					}
				}
				postProp->RemoveProperty(sRemoveProperties);
			}
			pDst->AddAllPropertiesFrom(postProp.get());
		}
	}
}

void PropertyListPostInitializer::LoadPostInitList()
{
	vector<uint32_t> instanceList {};
	PropManager.GetPropertyListIDs(sGroupID, instanceList);
	for (const auto instance : instanceList) {
		PropertyListPtr propList;
		PropManager.GetPropertyList(instance, sGroupID, propList);

		size_t stringCount = 0;
		Extensions::Property::array_string8 stringList;
		Extensions::Property::GetArrayString8(propList.get(), id("postinitList"), stringCount, stringList);
		for (size_t j = 0; j < stringCount; j++) {
			ArgScript::Line PropLine = ArgScript::Line(stringList[j].c_str());
			if (PropLine.GetArgumentsCount() != 2) continue;

			ResourceKey postinit, source;

			if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(0)), postinit)) continue;
			if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(1)), source)) continue;

			mPostInits.emplace_back(postinit, source);
		}
	}
}

bool PropertyListPostInitializer::GetTestResults(string& error_string) const
{
	error_string = mErrorString;
	return mErrorString.empty();
}

void PropertyListPostInitializer::Test()
{
	constexpr auto group_id = id("PropertyListPostInitializer");
	
	auto* base_property_list_parent = new App::PropertyList();
	{
		App::Property parent_property_prop;
		parent_property_prop.SetValueUInt32(17);
		base_property_list_parent->SetProperty(id("parent_property"), &parent_property_prop);
	}

	auto* postinit_property_list_parent = new App::PropertyList();
	{
		App::Property postinit_parent_property_prop;
		postinit_parent_property_prop.SetValueUInt32(12);
		postinit_property_list_parent->SetProperty(id("postinit_parent_property"), &postinit_parent_property_prop);
	}

	auto* base_property_list = new App::PropertyList();
	{
		base_property_list->SetParent(base_property_list_parent);

		App::Property remove_property_prop;
		remove_property_prop.SetValueBool(true);
		base_property_list->SetProperty(id("postinit_remove"), &remove_property_prop);

		App::Property replace_property_prop;
		replace_property_prop.SetValueInt32(1);
		base_property_list->SetProperty(id("postinit_replace"), &replace_property_prop);

		App::Property remain_property_prop;
		remain_property_prop.SetValueInt32(-2);
		base_property_list->SetProperty(id("postinit_remain"), &remain_property_prop);
	}
	
	uint32_t postinit_remove = id("postinit_remove");
	auto* postinit_property_list = new App::PropertyList();
	{
		App::Property new_parent_prop;
		new_parent_prop.SetValueKey({id("postinit_property_list_parent"), 0, group_id});
		postinit_property_list->SetProperty(sNewParent, &new_parent_prop);

		App::Property remove_properties_prop;
		remove_properties_prop.SetArrayUInt32(&postinit_remove, 1);
		postinit_property_list->SetProperty(sRemoveProperties, &remove_properties_prop);

		App::Property replace_property_prop;
		replace_property_prop.SetValueInt32(0);
		postinit_property_list->SetProperty(id("postinit_replace"), &replace_property_prop);

		App::Property add_property_prop;
		add_property_prop.SetValueBool(true);
		postinit_property_list->SetProperty(id("postinit_add"), &add_property_prop);
	}
	
	PropManager.AddPropertyList(base_property_list, id("base_property_list"), group_id);
	PropManager.AddPropertyList(base_property_list_parent, id("base_property_list_parent"), group_id);
	PropManager.AddPropertyList(postinit_property_list, id("postinit_property_list"), group_id);
	PropManager.AddPropertyList(postinit_property_list_parent, id("postinit_property_list_parent"), group_id);

	ResourceKey postinit{id("base_property_list"), 0, group_id};
	ResourceKey source{id("postinit_property_list"), 0, group_id};
	const auto postinit_pair = eastl::make_pair(postinit, source);
	mPostInits.push_back(postinit_pair);

	PropertyListPtr postinit_test;
	PropManager.GetPropertyList(id("base_property_list"), group_id, postinit_test);
	{
		App::Property* prop;
		if (postinit_test->GetProperty(id("parent_property"), prop) || !postinit_test->GetProperty(id("postinit_parent_property"), prop))
		{
			const eastl::string failure = "New Parent postinit failed";
			ModAPI::Log(failure.c_str());
			mErrorString += failure + "\n";
		}
		if (!postinit_test->GetProperty(id("postinit_remain"), prop))
		{
			const eastl::string failure = "Remain postinit failed";
			ModAPI::Log(failure.c_str());
			mErrorString += failure + "\n";
		}
		if (postinit_test->GetProperty(id("postinit_remove"), prop))
		{
			const eastl::string failure = "Remove postinit failed";
			ModAPI::Log(failure.c_str());
			mErrorString += failure + "\n";
		}
		if (!postinit_test->GetProperty(id("postinit_replace"), prop) || *prop->GetValueInt32() != 0)
		{
			const eastl::string failure = "Replace postinit failed";
			ModAPI::Log(failure.c_str());
			mErrorString += failure + "\n";
		}
		if (postinit_test->GetProperty(sNewParent, prop) || postinit_test->GetProperty(sRemoveProperties, prop))
		{
			const eastl::string failure = "Command cleanup postinit failed";
			ModAPI::Log(failure.c_str());
			mErrorString += failure + "\n";
		}
	}

	const auto it = eastl::find(mPostInits.begin(), mPostInits.end(), postinit_pair);
	if (it != mPostInits.end()) mPostInits.erase(it);

	PropManager.RemovePropertyList(base_property_list);
	PropManager.RemovePropertyList(base_property_list_parent);
	PropManager.RemovePropertyList(postinit_property_list);
	PropManager.RemovePropertyList(postinit_property_list_parent);
	ModAPI::Log("PropertyListPostInitializer Test Results -> %s", mErrorString.empty() ? "Passed" : "Failed");
}
	
virtual_detour(GetPropertyList_detour, App::cPropManager, App::IPropManager, bool(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst))
{
	bool detoured(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst) {
		if (!original_function(this, instanceID, groupID, pDst)) return false;
		if (PropertyListPostInitializer::Exists())
			GetPropertyListPostInitializer().ApplyPostInits(original_function, this, instanceID, groupID, pDst);
		return true;
	}
};

void PropertyListPostInitializer::AttachDetours()
{
	GetPropertyList_detour::attach(GetAddress(App::cPropManager, GetPropertyList));
}
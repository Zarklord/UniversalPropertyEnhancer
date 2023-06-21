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

#include "PropertyExt.h"

PropertyListPostInitializer& GetPropertyListPostInitializer()
{
	return PropertyListPostInitializer::Get();
}

PropertyListPostInitializer* PropertyListPostInitializer::mInstance = nullptr;
void PropertyListPostInitializer::Initialize()
{
	mInstance = new PropertyListPostInitializer();
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
		PropertyExt::array_string_8* stringList = nullptr;
		PropertyExt::GetArrayString8(propList.get(), sArgumentList, stringCount, stringList);
		for (size_t j = 0; j < stringCount; j++) {
			ArgScript::Line PropLine = ArgScript::Line(stringList[j].mBegin);
			if (PropLine.GetArgumentsCount() != 2) continue;

			ResourceKey postinit, source;

			if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(0)), postinit)) continue;
			if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(1)), source)) continue;

			mPostInits.emplace_back(postinit, source);
		}
	}
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
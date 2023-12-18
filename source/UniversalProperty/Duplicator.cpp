/****************************************************************************
* Copyright (C) 2022 Zarklord
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
#include "Duplicator.h"

#include <memory>

#include "Spore/App/cPropManager.h"
#include "Spore/Resource/cResourceManager.h"

#include "PropertyExt.h"

PropertyListDuplicator& GetPropertyListDuplicator()
{
	return PropertyListDuplicator::Get();
}

PropertyListDuplicator* PropertyListDuplicator::mInstance = nullptr;
void PropertyListDuplicator::Initialize()
{
	mInstance = new PropertyListDuplicator();
	mInstance->PostConstruct();
}

void PropertyListDuplicator::Finalize()
{
	delete mInstance;
	mInstance = nullptr;
}

PropertyListDuplicator& PropertyListDuplicator::Get()
{
	return *mInstance;
}

bool PropertyListDuplicator::Exists()
{
	return mInstance != nullptr;
}

PropertyListDuplicator::PropertyListDuplicator()
{
	LoadDuplicationLists();
}

void PropertyListDuplicator::PostConstruct()
{
	Test();
}

PropertyListDuplicator::~PropertyListDuplicator()
{
	mDuplicationMap.clear();
}

size_t PropertyListDuplicator::GetExtraRecordKeys(vector<ResourceKey>& dst, Resource::IKeyFilter* filter)
{
	size_t extra_keys = 0;

	for (const auto& key : mDuplicationMap)
	{
		if (filter->IsValid(key.first))
		{
			dst.push_back(key.second);
			++extra_keys;
		}
	}

	return extra_keys;
}

void PropertyListDuplicator::LoadDuplicationLists()
{
	vector<uint32_t> instanceList {};
	PropManager.GetPropertyListIDs(sGroupID, instanceList);
	for (const auto instance : instanceList) {
		PropertyListPtr propList;
		PropManager.GetPropertyList(instance, sGroupID, propList);

		size_t stringCount = 0;
		Extensions::Property::array_string8 stringList;
		Extensions::Property::GetArrayString8(propList.get(), sArgumentList, stringCount, stringList);
		for (size_t j = 0; j < stringCount; j++) {
			ArgScript::Line PropLine = ArgScript::Line(stringList[j]);
			if (PropLine.GetArgumentsCount() != 2) continue;

			ResourceKey newProp, sourceProp;

			eastl::string8 source_str = PropLine.GetArgumentAt(1);

			if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(0)), newProp)) continue;
			if (!App::Property::GetKey(propList.get(), id(source_str.c_str()), sourceProp))
			{
				source_str.pop_back();
				if (!App::Property::GetKey(propList.get(), id(source_str.c_str()), sourceProp)) continue;
			}
			if (PropManager.HasPropertyList(newProp.instanceID, newProp.groupID)) continue; //this CANNOT be used to replace files!

			newProp.typeID = TypeIDs::prop;
			sourceProp.typeID = TypeIDs::prop;
			
			if (mDuplicationMap.find(newProp) != mDuplicationMap.end()) continue;
			mDuplicationMap[newProp] = sourceProp;
		}
	}
}

bool PropertyListDuplicator::GetTestResults(string& error_string) const
{
	error_string = mErrorString;
	return mErrorString.empty();
}

void PropertyListDuplicator::Test()
{
	constexpr auto group_id = id("PropertyListDuplicator");

	const ResourceKey new_prop{id("fake_duplicator_prop"), TypeIDs::prop, group_id};
	const ResourceKey source_prop{id("real_source_prop"), TypeIDs::prop, group_id};
	mDuplicationMap[new_prop] = source_prop;

	eastl::vector<ResourceKey> resources;
	Resource::StandardFileFilter filter(id("fake_duplicator_prop"), group_id, TypeIDs::prop);
	ResourceManager.GetRecordKeyList(resources, &filter);

	if (eastl::find(resources.begin(), resources.end(), source_prop) == resources.end())
	{
		const eastl::string failure = "ResourceKey duplicator failed";
		ModAPI::Log(failure.c_str());
		mErrorString += failure + "\n";
	}

	mDuplicationMap.erase(new_prop);

	ModAPI::Log("PropertyListDuplicator Test Results -> %s", mErrorString.empty() ? "Passed" : "Failed");
}

virtual_detour(GetRecordKeyList_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, vector<Resource::Database*>* pDstDatabases))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, vector<Resource::Database*>* pDstDatabases)
	{
		size_t func_result = original_function(this, dst, filter, pDstDatabases);

		if (PropertyListDuplicator::Exists())
			func_result += GetPropertyListDuplicator().GetExtraRecordKeys(dst, filter);

		return func_result;
	}
};

virtual_detour(GetRecordKeyList2_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, bool))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, bool cached)
	{
		size_t func_result = original_function(this, dst, filter, cached);

		if (PropertyListDuplicator::Exists())
			func_result += GetPropertyListDuplicator().GetExtraRecordKeys(dst, filter);

		return func_result;
	}
};

void PropertyListDuplicator::AttachDetours() {
	GetRecordKeyList_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList));
	GetRecordKeyList2_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList2));
}

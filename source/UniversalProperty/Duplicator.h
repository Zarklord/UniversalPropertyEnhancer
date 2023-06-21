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

#pragma once
#include <Spore/BasicIncludes.h>

class PropertyListDuplicator
{
public:
	static void Initialize();
	static void Finalize();
	static PropertyListDuplicator& Get();
	static bool Exists();
	
	static constexpr uint32_t sGroupID = id("prop_duplications");
	static constexpr uint32_t sArgumentList = id("duplicateList");

	static void AttachDetours();
private:
	PropertyListDuplicator();
public:
	~PropertyListDuplicator();

	size_t GetExtraRecordKeys(vector<ResourceKey>& dst, Resource::IKeyFilter* filter);
private:
	void LoadDuplicationLists();

	static PropertyListDuplicator* mInstance;

	hash_map<ResourceKey, ResourceKey> mDuplicationMap;
};

PropertyListDuplicator& GetPropertyListDuplicator();
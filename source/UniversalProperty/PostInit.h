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

#pragma once

#include <Spore/BasicIncludes.h>
#include <Spore/App/cPropManager.h>


class PropertyListPostInitializer
{
public:
	static void Initialize();
	static void Finalize();
	static PropertyListPostInitializer& Get();
	static bool Exists();
	
	static constexpr uint32_t sGroupID = id("prop_postinits");
	static constexpr uint32_t sArgumentList = id("postinitList");
	
	static constexpr uint32_t sNewParent = id("newParent");
	static constexpr uint32_t sRemoveProperties = id("removeProperties");

	static void AttachDetours();
private:
	PropertyListPostInitializer();
public:
	~PropertyListPostInitializer();

	typedef bool (__thiscall *GetPropertyList)(App::IPropManager* this_ptr, uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst);
	void ApplyPostInits(GetPropertyList fn, App::IPropManager* this_ptr, uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst);
private:
	void LoadPostInitList();

	static PropertyListPostInitializer* mInstance;
	
	vector<pair<ResourceKey, ResourceKey>> mPostInits;
};

PropertyListPostInitializer& GetPropertyListPostInitializer();
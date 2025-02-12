/****************************************************************************
* Copyright (C) 2018-2024 Zarklord
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

#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

#include <Spore/App/cPropManager.h>

static LuaMultiReference<sol::function> sApplyPropertyListPostInit;

OnLuaInit(sol::state_view s, bool is_main_state)
{
	s["SetPropertyListPostInitFunction"] = [](const sol::this_state L, sol::function&& fn)
	{
		sApplyPropertyListPostInit.set(L, std::move(fn));
	};
}

OnLuaDispose(sol::state_view s, bool is_main_state)
{
	sApplyPropertyListPostInit.clear(s);
}

virtual_detour(GetPropertyList_detour, App::cPropManager, App::IPropManager, bool(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst))
{
	bool detoured(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst) //NOLINT
	{
		if (!original_function(this, instanceID, groupID, pDst)) return false;
		
		if (sApplyPropertyListPostInit)
		{
			auto free_state = GetLuaSpore().GetFreeLuaState();
			if (const auto* fn = sApplyPropertyListPostInit.get(free_state))
			{
				fn->call(pDst, instanceID, groupID);
			}
		}
		return true;
	}
};

AddSporeDetours()
{
	GetPropertyList_detour::attach(GetAddress(App::cPropManager, GetPropertyList));
}
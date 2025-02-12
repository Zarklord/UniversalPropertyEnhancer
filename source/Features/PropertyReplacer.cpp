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

static LuaMultiReference<sol::function> sApplyPropertyReplacerFunction;

OnLuaInit(sol::state_view s, bool is_main_state)
{
	s["SetPropertyReplacerFunction"] = [](const sol::this_state L, sol::function&& fn)
	{
		sApplyPropertyReplacerFunction.set(L, std::move(fn));
	};
}

OnLuaDispose(sol::state_view s, bool is_main_state)
{
	sApplyPropertyReplacerFunction.clear(s);
}

short kCustomPropertyFlagPropertyReplaced = 0x4000;
static bool GetPropertyReplaced(App::Property*& prop)
{
	return prop->mnFlags & kCustomPropertyFlagPropertyReplaced;
}
static void SetPropertyReplaced(App::Property*& prop)
{
	prop->mnFlags |= kCustomPropertyFlagPropertyReplaced;
}

member_detour(GetPropertyAlt_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result) //NOLINT
	{
		thread_local int depth = 0;
		++depth;
		const bool fresult = original_function(this, propertyID, result);
		if (fresult && depth == 1 && !GetPropertyReplaced(result) && sApplyPropertyReplacerFunction)
		{
			GetLuaSpore().ExecuteOnFreeState([this, propertyID, &result](const sol::state_view& s)
			{
				if (const auto* fn = sApplyPropertyReplacerFunction.get(s))
				{
					fn->call(propertyID, result, static_cast<App::PropertyList*>(this));
					SetPropertyReplaced(result);
				}
			});
		}
		--depth;
		return fresult;
	}
};

member_detour(GetProperty_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result) //NOLINT
	{
		thread_local int depth = 0;
		++depth;
		const bool fresult = original_function(this, propertyID, result);
		if (fresult && depth == 1 && !GetPropertyReplaced(result) && sApplyPropertyReplacerFunction)
		{
			GetLuaSpore().ExecuteOnFreeState([this, propertyID, &result](const sol::state_view& s)
			{
				if (const auto* fn = sApplyPropertyReplacerFunction.get(s))
				{
					fn->call(propertyID, result, static_cast<App::PropertyList*>(this));
					SetPropertyReplaced(result);
				}
			});
		}
		--depth;
		return fresult;
	}
};

member_detour(GetPropertyObject_detour, App::PropertyList, App::Property*(uint32_t propertyID))
{
	App::Property* detoured(uint32_t propertyID) //NOLINT
	{
		thread_local int depth = 0;
		++depth;
		App::Property* result = original_function(this, propertyID);
		if (depth == 1 && sApplyPropertyReplacerFunction)
		{
			GetLuaSpore().ExecuteOnFreeState([this, propertyID, &result](const sol::state_view& s)
			{
				if (const auto* fn = sApplyPropertyReplacerFunction.get(s))
				{
					fn->call(propertyID, result, static_cast<App::PropertyList*>(this));
				}
			});
		}
		--depth;
		return result;
	}
};

AddSporeDetours()
{
	GetPropertyAlt_detour::attach(GetAddress(App::PropertyList, GetPropertyAlt));
	GetProperty_detour::attach(GetAddress(App::PropertyList, GetProperty));
	GetPropertyObject_detour::attach(GetAddress(App::PropertyList, GetPropertyObject));
}
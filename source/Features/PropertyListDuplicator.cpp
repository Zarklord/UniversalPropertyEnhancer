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

#include <Spore/Resource/cResourceManager.h>

static LuaMultiReference<sol::function> sApplyPropertyListDuplicator;

OnLuaInit(sol::state_view s, bool is_main_state)
{
	s["SetPropertyListDuplicatorFunction"] = [](const sol::this_state L, sol::function&& fn)
	{
		sApplyPropertyListDuplicator.set(L, std::move(fn));
	};
}

OnLuaDispose(sol::state_view s, bool is_main_state)
{
	sApplyPropertyListDuplicator.clear(s);
}

virtual_detour(GetRecordKeyList_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, vector<Resource::Database*>* pDstDatabases))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, vector<Resource::Database*>* pDstDatabases) //NOLINT
	{
		size_t func_result = original_function(this, dst, filter, pDstDatabases);
		
		if (sApplyPropertyListDuplicator)
		{
			auto free_state = GetLuaSpore().GetFreeLuaState();

			if (const auto* fn = sApplyPropertyListDuplicator.get(free_state))
			{
				const auto result = fn->call(filter);
				if (!result.valid()) return func_result;

				const sol::table record_keys = result;
				const size_t record_keys_size = record_keys.size();

				func_result += record_keys_size;
				dst.reserve(func_result);
				record_keys.for_each([&dst](sol::object key, const sol::object& value)
				{
					dst.push_back(value.as<ResourceKey&>());
				});
			}
		}

		return func_result;
	}
};

virtual_detour(GetRecordKeyList2_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, bool))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, bool cached) //NOLINT
	{
		size_t func_result = original_function(this, dst, filter, cached);
		
		if (sApplyPropertyListDuplicator)
		{
			auto free_state = GetLuaSpore().GetFreeLuaState();
			if (const auto* fn = sApplyPropertyListDuplicator.get(free_state))
			{
				const auto result = fn->call(filter);
				if (!result.valid()) return func_result;

				const sol::table record_keys = result;
				const size_t record_keys_size = record_keys.size();

				func_result += record_keys_size;
				dst.reserve(func_result);
				record_keys.for_each([&dst](sol::object key, const sol::object& value)
				{
					dst.push_back(value.as<ResourceKey&>());
				});
			}
		}

		return func_result;
	}
};

AddSporeDetours()
{
	GetRecordKeyList_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList));
	GetRecordKeyList2_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList2));
}
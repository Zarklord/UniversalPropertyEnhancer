#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

#include <Spore/Resource/cResourceManager.h>

static sol::function sApplyPropertyListDuplicator;

OnLuaInit(sol::state_view s)
{
	s["SetPropertyListDuplicatorFunction"] = [](const sol::function& fn)
	{
		sApplyPropertyListDuplicator = fn;
	};
}

OnLuaDispose(sol::state_view s)
{
	sApplyPropertyListDuplicator.reset();
}

virtual_detour(GetRecordKeyList_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, vector<Resource::Database*>* pDstDatabases))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, vector<Resource::Database*>* pDstDatabases)
	{
		size_t func_result = original_function(this, dst, filter, pDstDatabases);
		
		LUA_THREAD_SAFETY();
		if (sApplyPropertyListDuplicator.valid())
		{
			const sol::table record_keys = sApplyPropertyListDuplicator(filter);
			const size_t record_keys_size = record_keys.size();

			func_result += record_keys_size;
			dst.reserve(func_result);
			record_keys.for_each([&dst](sol::object key, sol::object value)
			{
				dst.push_back(value.as<ResourceKey&>());
			});
		}

		return func_result;
	}
};

virtual_detour(GetRecordKeyList2_detour, Resource::cResourceManager, Resource::IResourceManager, size_t(vector<ResourceKey>&, Resource::IKeyFilter*, bool))
{
	size_t detoured(vector<ResourceKey>& dst, Resource::IKeyFilter* filter, bool cached)
	{
		size_t func_result = original_function(this, dst, filter, cached);
		
		if (sApplyPropertyListDuplicator.valid())
		{
			LUA_THREAD_SAFETY();
			const sol::table record_keys = sApplyPropertyListDuplicator(filter);
			const size_t record_keys_size = record_keys.size();

			func_result += record_keys_size;
			dst.reserve(func_result);
			record_keys.for_each([&dst](sol::object key, sol::object value)
			{
				dst.push_back(value.as<ResourceKey&>());
			});
		}

		return func_result;
	}
};

AddSporeDetours()
{
	GetRecordKeyList_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList));
	GetRecordKeyList2_detour::attach(GetAddress(Resource::cResourceManager, GetRecordKeyList2));
}
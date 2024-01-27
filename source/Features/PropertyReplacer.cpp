#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

static sol::function sApplyPropertyReplacerFunction;

OnLuaInit(sol::state_view s)
{
	s["SetPropertyReplacerFunction"] = [](const sol::function& fn)
	{
		sApplyPropertyReplacerFunction = fn;
	};
}

OnLuaDispose(sol::state_view s)
{
	sApplyPropertyReplacerFunction.reset();
}

member_detour(GetPropertyAlt_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result)
	{
		const bool fresult = original_function(this, propertyID, result);
		LUA_THREAD_SAFETY();
		if (fresult && sApplyPropertyReplacerFunction)
		{
			sApplyPropertyReplacerFunction(propertyID, result, static_cast<App::PropertyList*>(this));
		}
		return fresult;
	}
};

member_detour(GetProperty_detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result))
{
	bool detoured(uint32_t propertyID, App::Property*& result)
	{
		const bool fresult = original_function(this, propertyID, result);
		LUA_THREAD_SAFETY();
		if (fresult && sApplyPropertyReplacerFunction)
		{
			sApplyPropertyReplacerFunction(propertyID, result, static_cast<App::PropertyList*>(this));
		}
		return fresult;
	}
};

member_detour(GetPropertyObject_detour, App::PropertyList, App::Property*(uint32_t propertyID))
{
	App::Property* detoured(uint32_t propertyID)
	{
		App::Property* result = original_function(this, propertyID);
		LUA_THREAD_SAFETY();
		if (sApplyPropertyReplacerFunction)
		{
			sApplyPropertyReplacerFunction(propertyID, result, static_cast<App::PropertyList*>(this));
		}
		return result;
	}
};

AddSporeDetours()
{
	GetPropertyAlt_detour::attach(GetAddress(App::PropertyList, GetPropertyAlt));
	GetProperty_detour::attach(GetAddress(App::PropertyList, GetProperty));
	GetPropertyObject_detour::attach(GetAddress(App::PropertyList, GetPropertyObject));
}
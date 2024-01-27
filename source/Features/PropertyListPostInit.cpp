#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

#include <Spore/App/cPropManager.h>

virtual_detour(GetPropertyList_detour, App::cPropManager, App::IPropManager, bool(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst))
{
	bool detoured(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst)
	{
		if (!original_function(this, instanceID, groupID, pDst)) return false;
		LUA_THREAD_SAFETY();
		if (sApplyPropertyListPostInit.valid())
		{
			sApplyPropertyListPostInit(pDst, instanceID, groupID);
		}
		return true;
	}

	static inline sol::function sApplyPropertyListPostInit;
};

OnLuaInit(sol::state_view s)
{
	s["SetPropertyListPostInitFunction"] = [](const sol::function& fn)
	{
		GetPropertyList_detour::sApplyPropertyListPostInit = fn;
	};
}

OnLuaDispose(sol::state_view s)
{
	GetPropertyList_detour::sApplyPropertyListPostInit.reset();
}

AddSporeDetours()
{
	GetPropertyList_detour::attach(GetAddress(App::cPropManager, GetPropertyList));
}
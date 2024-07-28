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

#define UPE_MODNAME "UniversalPropertyEnhancer"
#define UPE_VERSION 10000

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		PrepareDetours(hModule);
		LuaSpore::RegisterCPPMod(UPE_MODNAME, UPE_VERSION);
		CommitDetours();
		break;
	case DLL_PROCESS_DETACH:
		*reinterpret_cast<uint32_t*>(GetAddress(Internal, Allocator_ptr)) = NULL;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}


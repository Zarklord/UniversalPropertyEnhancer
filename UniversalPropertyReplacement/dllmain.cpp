/****************************************************************************
* Copyright (C) 2018 Zarklord
*
* This file is part of UniversalPropertyReplacement.
*
* UniversalPropertyReplacement is free software: you can redistribute it and/or modify
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
* along with UniversalPropertyReplacement.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include <ModAPI\MainUtilities.h>
#include "UniversalPropertyReplacement.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	long error;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		// This line is always necessary
		ModAPI::ModAPIUtils::InitModAPI();

		ModAPI::ModAPIUtils::AddInitFunction(UniversalPropReplacement::Inititalize);

		PrepareDetours(hModule);
		// It is recommended to attach the detoured methods in specialised methods in the class
		UniversalPropReplacement::AttachDetour();
		error = SendDetours();

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


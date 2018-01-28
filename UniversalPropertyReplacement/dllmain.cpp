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


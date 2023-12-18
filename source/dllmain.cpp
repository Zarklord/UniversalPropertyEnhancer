/****************************************************************************
* Copyright (C) 2018, 2020 Zarklord
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

// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "UniversalProperty/Duplicator.h"
#include "UniversalProperty/PostInit.h"
#include "UniversalProperty/Replacement.h"
#include "PaletteIcons.h"
#include "VerifyUPE.h"

void Main()
{
}

void Initialize()
{
#ifdef _DEBUG
	ManualBreakpoint();
#endif

	PropertyListDuplicator::Initialize();
	PropertyListPostInitializer::Initialize();
	UniversalPropertyReplacer::Initialize();
}

void PostInitialize()
{
	CheatManager.AddCheat("VerifyUPE", new VerifyUPE());

	ModAPI::Log("Universal Property Enhancer " UPE_VERSION_STRING " Loaded");
}

void Dispose()
{
	PropertyListDuplicator::Finalize();
	PropertyListPostInitializer::Finalize();
	UniversalPropertyReplacer::Finalize();
}

void AttachDetours()
{
	PropertyListDuplicator::AttachDetours();
	PropertyListPostInitializer::AttachDetours();
	UniversalPropertyReplacer::AttachDetours();
	PaletteIcons::AttachDetours();
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddInitFunction(Initialize);
		ModAPI::AddPostInitFunction(PostInitialize);
		ModAPI::AddDisposeFunction(Dispose);
		
		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}


/****************************************************************************
* Copyright (C) 2020, 2022 Zarklord
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

#include "stdafx.h"
#include "PaletteIcons.h"

namespace PaletteIcons {
	member_detour(LoadPaletteIconProps_detour, PaletteIconsClass, void())
	{
		void detoured() {
			original_function(this);
			
			uint32_t groupID = id("paletteicons");
			vector<uint32_t> instanceList {};
			PropManager.GetPropertyListIDs(groupID, instanceList);

<<<<<<< master:UniversalPropertyEnhancer/PaletteIcons.cpp
		return result;
	}
};

using namespace PaletteIcons;

void PaletteIcons::LoadPaletteIconProps__detour::DETOUR() {

	original_function(this);
	
	uint32_t groupID = id("paletteicons");
	eastl::vector<uint32_t> instanceList {};
	PropManager.GetPropertyListIDs(groupID, instanceList);
=======
			for (eastl_size_t i = 0; i < instanceList.size(); i++) {
				PropertyListPtr propList;
				if (!PropManager.GetPropertyList(instanceList[i], groupID, propList)) continue;
				PaletteData* palettedata = new PaletteData {};
				palettedata->ID = instanceList[i];
				if (!App::Property::GetInt32(propList.get(), paletteSetSequenceNumber, palettedata->sequence)) { delete palettedata; continue; }
				if (!App::Property::GetKeyInstanceID(propList.get(), paletteSetButtonLayout, palettedata->layoutID)) { delete palettedata; continue; }
				if (!App::Property::GetKey(propList.get(), paletteSetButtonIcon, palettedata->icon)) { delete palettedata; continue; }
				App::Property::GetKey(propList.get(), paletteSetButtonBackgroundIcon, palettedata->bgicon);
				App::Property::GetText(propList.get(), paletteSetName, palettedata->name);
				vector<PaletteData*>::iterator j;
				for (j = this->paletteicons.begin(); j != this->paletteicons.end() && palettedata->sequence >= (*j)->sequence; j++) {}
				this->paletteicons.insert(j, palettedata);
			}
		}
	};
>>>>>>> current wip:source/PaletteIcons.cpp

	void AttachDetours() {
		LoadPaletteIconProps_detour::attach(Address(ModAPI::ChooseAddress(0x7DBBA0, 0x7DB710)));
	}
}
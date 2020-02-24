/****************************************************************************
* Copyright (C) 2020 Zarklord
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
	long AttachDetours() {
		long result = 0;
		result |= LoadPaletteIconProps__detour::attach(Address(ModAPI::ChooseAddress(0x0, 0x7DB710)));

		return result;
	}
	auto_METHOD_VOID(PaletteIconsClass, storepaletteicondata, Args(PaletteData** src, PaletteData** dst), Args(src, dst));
};

using namespace PaletteIcons;

void PaletteIcons::LoadPaletteIconProps__detour::DETOUR() {

	original_function(this);
	
	uint32_t groupID = id("paletteicons");
	eastl::vector<uint32_t> instanceList {};
	PropManager.GetAllListIDs(groupID, instanceList);

	for (eastl_size_t i = 0; i < instanceList.size(); i++) {
		PropertyListPtr propList;
		if (!PropManager.GetPropertyList(instanceList[i], groupID, propList)) continue;
		PaletteData* palettedata = new PaletteData {};
		palettedata->ID = instanceList[i];
		if (!App::Property::GetInt32(propList.get(), 0x5D1A718, palettedata->sequence)) continue; /* paletteSetSequenceNumber */
		if (!App::Property::GetKeyInstanceID(propList.get(), 0x05DFEF47, palettedata->layoutID)) continue; /* paletteSetButtonLayout */
		if (!App::Property::GetKey(propList.get(), 0x05E4DE4E, palettedata->icon)) continue; /* paletteSetButtonIcon */
		if (!App::Property::GetKey(propList.get(), 0x05DFEF48, palettedata->bgicon)) continue; /* paletteSetButtonBackgroundIcon */
		App::Property::GetText(propList.get(), 0xFE1E2BC2, palettedata->name); /* paletteSetName */
		PaletteData** idxptr = this->startptr;
		while (idxptr < this->currentidxptr && palettedata->sequence >= (*idxptr)->sequence) {
			idxptr++;
		}
		if (idxptr == this->currentidxptr && this->currentidxptr < this->endptr) {
			*this->currentidxptr = palettedata;
			this->currentidxptr++;
		} else {
			this->storepaletteicondata(idxptr, &palettedata);
		}
	}
}
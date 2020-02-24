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

#pragma once
#include <Spore/BasicIncludes.h>

namespace PaletteIcons {
	long AttachDetours();

	struct PaletteData {
		uint32_t ID;
		int32_t sequence;
		uint32_t layoutID;
		ResourceKey icon;
		ResourceKey bgicon;
		LocalizedString name;
	};

	class PaletteIconsClass {
		public:
			/* 00h */ PaletteData** startptr;
			/* 04h */ PaletteData** currentidxptr;
			/* 08h */ PaletteData** endptr;
			void storepaletteicondata(PaletteData** src, PaletteData** dst);
	};
	member_detour(LoadPaletteIconProps__detour, PaletteIconsClass, void()) {};
	namespace Addresses(PaletteIconsClass) {
		DefineAddress(storepaletteicondata, ModAPI::ChooseAddress(0x67F380, 0x690B40));
	};
};


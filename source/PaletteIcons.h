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

#pragma once
#include <Spore/BasicIncludes.h>

namespace PaletteIcons {
	void AttachDetours();

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
			vector<PaletteData*> paletteicons;
	};
	
	constexpr uint32_t paletteSetSequenceNumber		  = 0x5D1A718;
	constexpr uint32_t paletteSetButtonLayout		  = 0x05DFEF47;
	constexpr uint32_t paletteSetButtonIcon			  = 0x05E4DE4E;
	constexpr uint32_t paletteSetButtonBackgroundIcon = 0x05DFEF48;
	constexpr uint32_t paletteSetName				  = 0xFE1E2BC2;
};
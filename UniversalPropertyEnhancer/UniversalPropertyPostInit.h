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
#include <Spore/App/cPropManager.h>

namespace UniversalPropertyPostInit {
	bool Inititalize();
	long AttachDetours();

	static eastl::vector<eastl::pair<ResourceKey, ResourceKey>> postinits {};

	virtual_detour(GetPropertyList__detour, App::cPropManager, App::IPropManager, bool(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst)) {};
};
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
#include "UniversalPropertyPostInit.h"

namespace UniversalPropertyPostInit {
	long AttachDetours() {
		long result = 0;
		result |= GetPropertyList__detour::attach(GetAddress(App::cPropManager, GetPropertyList));

		return result;
	}
	bool Inititalize() {
		uint32_t groupID = id("prop_postinits");
		eastl::vector<uint32_t> instanceList {};
		PropManager.GetAllListIDs(groupID, instanceList);
		for (eastl_size_t i = 0; i < instanceList.size(); i++) {
			PropertyListPtr propList;
			PropManager.GetPropertyList(instanceList[i], groupID, propList);

			size_t stringCount = 0;
			eastl::string8* stringList = nullptr;
			App::Property::GetArrayString8(propList.get(), id("postinitList"), stringCount, stringList);

			for (size_t j = 0; j < stringCount; j++) {
				ResourceKey postinit {};
				ResourceKey source {};

				//as much as GetArrayString8 says it gives a string8 array it doesn't, this took alot of debugging to get this right, the strings are seperated every 0x8 bytes as pointers to the strings.
				ArgScript::Line PropLine = ArgScript::Line(*(char**)(static_cast<char*>(static_cast<void*>(stringList)) + (0x8 * j)));
				if (PropLine.GetArgumentsCount() != 2) continue;

				if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(0)), postinit)) continue;
				if (!App::Property::GetKey(propList.get(), id(PropLine.GetArgumentAt(1)), source)) continue;

				postinits.emplace_back(eastl::pair<ResourceKey, ResourceKey>{postinit, source});
			}
		}
		return true;
	}
}

using namespace UniversalPropertyPostInit;

template<typename FN, typename T>
void ApplyPostInits(FN* original_function, T* ClassPtr, uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst) {
	for (auto i : postinits) {
		if (i.first.groupID == groupID && i.first.instanceID == instanceID) {
			PropertyListPtr postProp;
			if (!original_function(ClassPtr, i.second.instanceID, i.second.groupID, postProp)) continue;
			pDst->AddAllPropertiesFrom(postProp.get());
		}
	}
}

bool UniversalPropertyPostInit::GetPropertyList__detour::DETOUR(uint32_t instanceID, uint32_t groupID, PropertyListPtr& pDst) {
	if (!original_function(this, instanceID, groupID, pDst)) return false;
	ApplyPostInits(original_function, this, instanceID, groupID, pDst);
	return true;
}
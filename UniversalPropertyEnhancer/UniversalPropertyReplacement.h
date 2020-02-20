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

#pragma once
#include <Spore/BasicIncludes.h>

namespace UniversalPropertyReplacement {
	bool Inititalize();
	long AttachDetours();

	static eastl::map<uint32_t, bool> deletePropertyIDs {};

	static eastl::map<uint32_t, bool> boolValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<bool*, uint32_t>> boolValueMapArrayOverride {};

	static eastl::map<uint32_t, int32_t> int32ValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<int32_t*, uint32_t>> int32ValueMapArrayOverride {};

	static eastl::map<uint32_t, uint32_t> uint32ValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<uint32_t*, uint32_t>> uint32ValueMapArrayOverride {};

	static eastl::map<uint32_t, float> floatValueMapOverride{};
	static eastl::map<uint32_t, eastl::pair<float*, uint32_t>> floatValueMapArrayOverride {};

	static eastl::map<uint32_t, eastl::string> string8ValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<eastl::string*, uint32_t>> string8ValueMapArrayOverride {};

	static eastl::map<uint32_t, eastl::string16> string16ValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<eastl::string16*, uint32_t>> string16ValueMapArrayOverride {};

	static eastl::map<uint32_t, ResourceKey> keyValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<ResourceKey*, uint32_t>> keyValueMapArrayOverride {};

	static eastl::map<uint32_t, LocalizedString> textValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<LocalizedString*, uint32_t>> textValueMapArrayOverride {};

	static eastl::map<uint32_t, Vector2> vector2ValueMapOverride{};
	static eastl::map<uint32_t, eastl::pair<Vector2*, uint32_t>> vector2ValueMapArrayOverride {};

	static eastl::map<uint32_t, Vector3> vector3ValueMapOverride{};
	static eastl::map<uint32_t, eastl::pair<Vector3*, uint32_t>> vector3ValueMapArrayOverride {};

	static eastl::map<uint32_t, Vector4> vector4ValueMapOverride{};
	static eastl::map<uint32_t, eastl::pair<Vector4*, uint32_t>> vector4ValueMapArrayOverride {};

	static eastl::map<uint32_t, ColorRGB> colorRGBValueMapOverride{};
	static eastl::map<uint32_t, eastl::pair<ColorRGB*, uint32_t>> colorRGBValueMapArrayOverride {};

	static eastl::map<uint32_t, ColorRGBA> colorRGBAValueMapOverride {};
	static eastl::map<uint32_t, eastl::pair<ColorRGBA*, uint32_t>> colorRGBAValueMapArrayOverride {};

	static eastl::map<uint32_t, eastl::pair<Transform*, uint32_t>> transformValueMapArrayOverride {};

	static eastl::map<uint32_t, eastl::pair<BoundingBox*, uint32_t>> bBoxValueMapArrayOverride {};

	member_detour(GetPropertyAlt__detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result)) {};
	member_detour(GetProperty__detour, App::PropertyList, bool(uint32_t propertyID, App::Property*& result)) {};
	member_detour(GetPropertyObject__detour, App::PropertyList, App::Property*(uint32_t propertyID)) {};
};


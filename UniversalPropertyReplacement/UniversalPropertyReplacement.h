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

#pragma once

#include <Spore\Detouring.h>
#include <Spore\App\PropertyList.h>
#include <Spore\App\IPropManager.h>
#include <Spore\App\Property.h>

#define DetouredMethodStaticFixed(name, returnType, newName, parameters) typedef returnType (* name##_original_t)(parameters); \
	static name##_original_t name##_original; \
	static returnType newName (parameters);

class UniversalPropReplacement {
public:
	static long AttachDetours();
	static bool Inititalize();
};
static eastl::vector<eastl::pair<uint32_t, bool>> boolValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, bool>> boolValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, float>> floatValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, float>> floatValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, int32_t>> int32ValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, int32_t>> int32ValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, uint32_t>> uint32ValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, uint32_t>> uint32ValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, Vector2>> vector2ValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, Vector2>> vector2ValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, Vector3>> vector3ValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, Vector3>> vector3ValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, Vector4>> vector4ValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, Vector4>> vector4ValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, ColorRGB>> colorRGBValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, ColorRGB>> colorRGBValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, ColorRGBA>> colorRGBAValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, ColorRGBA>> colorRGBAValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, ResourceKey>> resourceKeyValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, ResourceKey>> resourceKeyValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, eastl::string>> stringValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, eastl::string>> stringValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, char *>> cstringValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, char *>> cstringValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, eastl::string16>> wstringValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, eastl::string16>> wstringValueMapOverride{};

static eastl::vector<eastl::pair<uint32_t, wchar_t *>> wcstringValueMapDefault{};
static eastl::vector<eastl::pair<uint32_t, wchar_t *>> wcstringValueMapOverride{};

template<typename method_t,typename T>
static bool DetouredGetProp(method_t &method, eastl::vector<eastl::pair<uint32_t, T>> &override, eastl::vector<eastl::pair<uint32_t, T>> &default, const App::PropertyList * pPropertyList, uint32_t propertyID, T &dst);


#pragma DisableOptimization
DetouredMethodStaticFixed(GetBool, bool, DetouredGetBool,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, bool &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetFloat, bool, DetouredGetFloat,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, float &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetInt32, bool, DetouredGetInt32,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, int32_t &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetUInt32, bool, DetouredGetUInt32,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, uint32_t &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetVector2, bool, DetouredGetVector2,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector2 &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetVector3, bool, DetouredGetVector3,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector3 &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetVector4, bool, DetouredGetVector4,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector4 &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetColorRGB, bool, DetouredGetColorRGB,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, ColorRGB &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetColorRGBA, bool, DetouredGetColorRGBA,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, ColorRGBA &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetKey, bool, DetouredGetKey,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, ResourceKey &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetString8, bool, DetouredGetString8,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, eastl::string &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetCString8, bool, DetouredGetCString8,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, char * &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetString16, bool, DetouredGetString16,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, eastl::string16 &dst));

#pragma DisableOptimization
DetouredMethodStaticFixed(GetCString16, bool, DetouredGetCString16,
						  PARAMS(const App::PropertyList *pPropertyList, uint32_t propertyID, wchar_t * &dst));
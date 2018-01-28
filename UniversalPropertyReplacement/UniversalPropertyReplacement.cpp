// Universal_Property_Replacement.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "UniversalPropertyReplacement.h"
//#include "TestingCheat.h"
#include <Spore\Hash.h>
#include <Spore\Cheats.h>
#include <Spore\ArgScript.h>
#include <limits>
#include <string>
#include <cwchar>
#include <iostream>


long UniversalPropReplacement::AttachDetour() {
	SetDetourAddress(GetBool, GetMethodAddress(App::Property, GetBool));
	SetDetourAddress(GetFloat, GetMethodAddress(App::Property, GetFloat));
	SetDetourAddress(GetInt32, GetMethodAddress(App::Property, GetInt32));
	SetDetourAddress(GetUInt32, GetMethodAddress(App::Property, GetUInt32));
	SetDetourAddress(GetVector2, GetMethodAddress(App::Property, GetVector2));
	SetDetourAddress(GetVector3, GetMethodAddress(App::Property, GetVector3));
	SetDetourAddress(GetVector4, GetMethodAddress(App::Property, GetVector4));
	SetDetourAddress(GetColorRGB, GetMethodAddress(App::Property, GetColorRGB));
	SetDetourAddress(GetColorRGBA, GetMethodAddress(App::Property, GetColorRGBA));
	SetDetourAddress(GetKey, GetMethodAddress(App::Property, GetKey));
	SetDetourAddress(GetString8, GetMethodAddress(App::Property, GetString8));
	SetDetourAddress(GetCString8, GetMethodAddress(App::Property, GetCString8));
	SetDetourAddress(GetString16, GetMethodAddress(App::Property, GetString16));
	SetDetourAddress(GetCString16, GetMethodAddress(App::Property, GetCString16));

	long result = AttachDetourFunctionStatic(GetBool_original, DetouredGetBool);
	result |= AttachDetourFunctionStatic(GetFloat_original, DetouredGetFloat);
	result |= AttachDetourFunctionStatic(GetInt32_original, DetouredGetInt32);
	result |= AttachDetourFunctionStatic(GetUInt32_original, DetouredGetUInt32);
	result |= AttachDetourFunctionStatic(GetVector2_original, DetouredGetVector2);
	result |= AttachDetourFunctionStatic(GetVector3_original, DetouredGetVector3);
	result |= AttachDetourFunctionStatic(GetVector4_original, DetouredGetVector4);
	result |= AttachDetourFunctionStatic(GetColorRGB_original, DetouredGetColorRGB);
	result |= AttachDetourFunctionStatic(GetColorRGBA_original, DetouredGetColorRGBA);
	result |= AttachDetourFunctionStatic(GetKey_original, DetouredGetKey);
	result |= AttachDetourFunctionStatic(GetString8_original, DetouredGetString8);
	result |= AttachDetourFunctionStatic(GetCString8_original, DetouredGetCString8);
	result |= AttachDetourFunctionStatic(GetString16_original, DetouredGetString16);
	result |= AttachDetourFunctionStatic(GetCString16_original, DetouredGetCString16);
	// You can compare result to NO_ERROR to see if there was any error. Or you can just ignore it.
	return result;
}

bool UniversalPropReplacement::Inititalize() {
	//App::ICheatManager::Get()->AddCheat("runTests", new TestingCheat());
	uint32_t groupID = Hash::FNV("prop_overrides");
	eastl::vector<uint32_t> instanceList{};
	App::IPropManager::Get()->GetAllListIDs(groupID, instanceList);

	for (eastl_size_t i = 0; i < instanceList.size(); i++) {
		App::PropertyList::Pointer pProp = nullptr;  // this is just an intrusive_ptr
		App::IPropManager::Get()->GetPropertyList(instanceList[i], groupID, pProp);

		size_t stringCount = 0;
		eastl::string8 * stringList = nullptr;
		App::Property::GetArrayString8(pProp.get(), Hash::FNV("replacementList"), stringCount, stringList);
		for (size_t j = 0; j < stringCount; j++) {
			bool result = false;
			bool override = false;
			eastl::string typeString{};
			uint32_t replaceHash = 0;
			eastl::string valueString{};

			//as much as GetArrayString8 says it gives a string8 array it doesn't, this took alot of debugging to get this right, the strings are seperated every 0x8 bytes as pointers to the strings.
			ArgScript::Line PropLine = ArgScript::Line(*(char**) (static_cast<char*>(static_cast<void*>(stringList)) + (0x8 * j)));
			if (PropLine.GetArgumentsCount() < 3 && PropLine.GetArgumentsCount() > 4) continue;
			typeString = PropLine.GetArgumentAt(0);
			replaceHash = std::strtoul(PropLine.GetArgumentAt(1), nullptr, 16);
			valueString = PropLine.GetArgumentAt(2);
			override = PropLine.HasFlag("override");

			typeString.make_lower();

			if (typeString == "bool") {
				bool value;
				result = App::Property::GetBool(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					boolValueMapOverride.emplace_back(eastl::pair<uint32_t, bool>{replaceHash, value});
				} else {
					boolValueMapDefault.emplace_back(eastl::pair<uint32_t, bool>{replaceHash, value});
				}

			} else if (typeString == "float") {
				float value;
				result = App::Property::GetFloat(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					floatValueMapOverride.emplace_back(eastl::pair<uint32_t, float>{replaceHash, value});
				} else {
					floatValueMapDefault.emplace_back(eastl::pair<uint32_t, float>{replaceHash, value});
				}

			} else if (typeString == "int32") {
				int32_t value;
				result = App::Property::GetInt32(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					int32ValueMapOverride.emplace_back(eastl::pair<uint32_t, int32_t>{replaceHash, value});
				} else {
					int32ValueMapDefault.emplace_back(eastl::pair<uint32_t, int32_t>{replaceHash, value});
				}


			} else if (typeString == "uint32") {
				uint32_t value;
				result = App::Property::GetUInt32(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					uint32ValueMapOverride.emplace_back(eastl::pair<uint32_t, uint32_t>{replaceHash, value});
				} else {
					uint32ValueMapDefault.emplace_back(eastl::pair<uint32_t, uint32_t>{replaceHash, value});
				}


			} else if (typeString == "vector2") {
				Vector2 value;
				result = App::Property::GetVector2(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					vector2ValueMapOverride.emplace_back(eastl::pair<uint32_t, Vector2>{replaceHash, value});
				} else {
					vector2ValueMapDefault.emplace_back(eastl::pair<uint32_t, Vector2>{replaceHash, value});
				}


			} else if (typeString == "vector3") {
				Vector3 value;
				result = App::Property::GetVector3(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					vector3ValueMapOverride.emplace_back(eastl::pair<uint32_t, Vector3>{replaceHash, value});
				} else {
					vector3ValueMapDefault.emplace_back(eastl::pair<uint32_t, Vector3>{replaceHash, value});
				}


			} else if (typeString == "vector4") {
				Vector4 value;
				result = App::Property::GetVector4(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					vector4ValueMapOverride.emplace_back(eastl::pair<uint32_t, Vector4>{replaceHash, value});
				} else {
					vector4ValueMapDefault.emplace_back(eastl::pair<uint32_t, Vector4>{replaceHash, value});
				}


			} else if (typeString == "colorrgb") {
				ColorRGB value;
				result = App::Property::GetColorRGB(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					colorRGBValueMapOverride.emplace_back(eastl::pair<uint32_t, ColorRGB>{replaceHash, value});
				} else {
					colorRGBValueMapDefault.emplace_back(eastl::pair<uint32_t, ColorRGB>{replaceHash, value});
				}


			} else if (typeString == "colorrgba") {
				ColorRGBA value;
				result = App::Property::GetColorRGBA(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					colorRGBAValueMapOverride.emplace_back(eastl::pair<uint32_t, ColorRGBA>{replaceHash, value});
				} else {
					colorRGBAValueMapDefault.emplace_back(eastl::pair<uint32_t, ColorRGBA>{replaceHash, value});
				}


			} else if (typeString == "key") {
				ResourceKey value;
				result = App::Property::GetKey(pProp.get(), Hash::FNV(valueString.c_str()), value);
				if (!result) continue;
				if (override) {
					resourceKeyValueMapOverride.emplace_back(eastl::pair<uint32_t, ResourceKey>{replaceHash, value});
				} else {
					resourceKeyValueMapDefault.emplace_back(eastl::pair<uint32_t, ResourceKey>{replaceHash, value});
				}


			} else if (typeString == "string8") {
				eastl::string value;
				char * cvalue;
				result = App::Property::GetString8(pProp.get(), Hash::FNV(valueString.c_str()), value);
				result |= App::Property::GetCString8(pProp.get(), Hash::FNV(valueString.c_str()), cvalue);
				if (!result) continue;
				if (override) {
					stringValueMapOverride.emplace_back(eastl::pair<uint32_t, eastl::string>{replaceHash, value});
					cstringValueMapOverride.emplace_back(eastl::pair<uint32_t, char *>{replaceHash, cvalue});
				} else {
					stringValueMapDefault.emplace_back(eastl::pair<uint32_t, eastl::string>{replaceHash, value});
					cstringValueMapDefault.emplace_back(eastl::pair<uint32_t, char *>{replaceHash, cvalue});
				}


			} else if (typeString == "string16") {
				eastl::string16 value;
				wchar_t * cvalue;
				result = App::Property::GetString16(pProp.get(), Hash::FNV(valueString.c_str()), value);
				result |= App::Property::GetCString16(pProp.get(), Hash::FNV(valueString.c_str()), cvalue);
				if (!result) continue;
				if (override) {
					wstringValueMapOverride.emplace_back(eastl::pair<uint32_t, eastl::string16>{replaceHash, value});
					wcstringValueMapOverride.emplace_back(eastl::pair<uint32_t, wchar_t *>{replaceHash, cvalue});
				} else {
					wstringValueMapDefault.emplace_back(eastl::pair<uint32_t, eastl::string16>{replaceHash, value});
					wcstringValueMapDefault.emplace_back(eastl::pair<uint32_t, wchar_t *>{replaceHash, cvalue});
				}

			}
		}
	}
	return true;
}

template<typename method_t, typename T>
bool DetouredGetProp(method_t &method, eastl::vector<eastl::pair<uint32_t, T>> &override, eastl::vector<eastl::pair<uint32_t, T>> &default, const App::PropertyList * pPropertyList, uint32_t propertyID, T &dst) {
	for (size_t i = 0; i < override.size(); i++) {
		if (propertyID == override[i].first) {
			dst = override[i].second;
			return true;
		}
	}
	bool result = method(pPropertyList, propertyID, dst);
	if (result == false) {
		for (size_t i = 0; i < default.size(); i++) {
			if (propertyID == default[i].first) {
				dst = default[i].second;
				return true;
			}
		}
	}
	return result;
}


bool DetouredGetBool(const App::PropertyList *pPropertyList, uint32_t propertyID, bool &dst) {
	return DetouredGetProp(GetBool_original, boolValueMapOverride, boolValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetFloat(const App::PropertyList *pPropertyList, uint32_t propertyID, float &dst) {
	return DetouredGetProp(GetFloat_original, floatValueMapOverride, floatValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetInt32(const App::PropertyList *pPropertyList, uint32_t propertyID, int32_t &dst) {
	return DetouredGetProp(GetInt32_original, int32ValueMapOverride, int32ValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetUInt32(const App::PropertyList *pPropertyList, uint32_t propertyID, uint32_t &dst) {
	return DetouredGetProp(GetUInt32_original, uint32ValueMapOverride, uint32ValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetVector2(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector2 &dst) {
	return DetouredGetProp(GetVector2_original, vector2ValueMapOverride, vector2ValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetVector3(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector3 &dst) {
	return DetouredGetProp(GetVector3_original, vector3ValueMapOverride, vector3ValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetVector4(const App::PropertyList *pPropertyList, uint32_t propertyID, Vector4 &dst) {
	return DetouredGetProp(GetVector4_original, vector4ValueMapOverride, vector4ValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetColorRGB(const App::PropertyList *pPropertyList, uint32_t propertyID, ColorRGB &dst) {
	return DetouredGetProp(GetColorRGB_original, colorRGBValueMapOverride, colorRGBValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetColorRGBA(const App::PropertyList *pPropertyList, uint32_t propertyID, ColorRGBA &dst) {
	return DetouredGetProp(GetColorRGBA_original, colorRGBAValueMapOverride, colorRGBAValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetKey(const App::PropertyList *pPropertyList, uint32_t propertyID, ResourceKey &dst) {
	return DetouredGetProp(GetKey_original, resourceKeyValueMapDefault, resourceKeyValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetString8(const App::PropertyList *pPropertyList, uint32_t propertyID, eastl::string &dst) {
	return DetouredGetProp(GetString8_original, stringValueMapOverride, stringValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetCString8(const App::PropertyList *pPropertyList, uint32_t propertyID, char* &dst) {
	return DetouredGetProp(GetCString8_original, cstringValueMapOverride, cstringValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetString16(const App::PropertyList *pPropertyList, uint32_t propertyID, eastl::string16 &dst) {
	ArgScript::PrintF(App::ICheatManager::GetConsoleStream(), "%u", wstringValueMapOverride.size());
	return DetouredGetProp(GetString16_original, wstringValueMapOverride, wstringValueMapDefault, pPropertyList, propertyID, dst);
}

bool DetouredGetCString16(const App::PropertyList *pPropertyList, uint32_t propertyID, wchar_t * &dst) {
	return DetouredGetProp(GetCString16_original, wcstringValueMapOverride, wcstringValueMapDefault, pPropertyList, propertyID, dst);
}
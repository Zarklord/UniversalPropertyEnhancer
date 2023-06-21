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

#include "stdafx.h"
#include "UniversalPropertyReplacement.h"
#include "Property.h"

namespace UniversalPropertyReplacement {
	long AttachDetours() {
		long result = 0;
		result |= GetPropertyAlt__detour::attach(GetAddress(App::PropertyList, GetPropertyAlt));
		result |= GetProperty__detour::attach(GetAddress(App::PropertyList, GetProperty));
		result |= GetPropertyObject__detour::attach(GetAddress(App::PropertyList, GetPropertyObject));

		return result;
	}
	bool Inititalize() {
		uint32_t groupID = id("prop_overrides");
		eastl::vector<uint32_t> instanceList{};
		PropManager.GetPropertyListIDs(groupID, instanceList);

		for (eastl_size_t i = 0; i < instanceList.size(); i++) {
			PropertyListPtr propList;
			PropManager.GetPropertyList(instanceList[i], groupID, propList);

			size_t stringCount = 0;
			eastl::string8* stringList = nullptr;
			Extensions::Property::array_string_8* altStringList = nullptr;
			App::Property::GetArrayString8(propList.get(), id("replacementList"), stringCount, stringList);
			Extensions::Property::GetArrayString8(propList.get(), id("replacementList"), stringCount, altStringList);
			const bool is_bad_array_string = stringCount > 1 && altStringList[1].mEnd >= altStringList[1].mBegin;
			for (size_t j = 0; j < stringCount; j++) {
				bool result = false;
				union AppExtProperty
				{
					Extensions::Property* ext;
					App::Property* prop;
				};
				AppExtProperty out{nullptr};
				uint32_t size;
				eastl::string typeString{};
				uint32_t replaceHash = 0;
				eastl::string valueString{};
				
				ArgScript::Line PropLine = ArgScript::Line(is_bad_array_string ? altStringList[j].mBegin : stringList[j].c_str());
				if (PropLine.GetArgumentsCount() > 2 && PropLine.GetArgumentsCount() > 4) continue;
				typeString = PropLine.GetArgumentAt(0);
				typeString.make_lower();

				replaceHash = std::strtoul(PropLine.GetArgumentAt(1), nullptr, 16);
				if (typeString == "delete") {
					deletePropertyIDs[replaceHash] = true;
					continue;
				}
				valueString = PropLine.GetArgumentAt(2);

				if (typeString == "bool") {
					bool value;
					result = App::Property::GetBool(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					boolValueMapOverride[replaceHash] = value;
				} else if (typeString == "int32") {
					int32_t value;
					result = App::Property::GetInt32(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					int32ValueMapOverride[replaceHash] = value;
				} else if (typeString == "uint32") {
					uint32_t value;
					result = App::Property::GetUInt32(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					uint32ValueMapOverride[replaceHash] = value;
				} else if (typeString == "float") {
					float value;
					result = App::Property::GetFloat(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					floatValueMapOverride[replaceHash] = value;
				} else if (typeString == "string8") {
					eastl::string value;
					result = App::Property::GetString8(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					string8ValueMapOverride[replaceHash] = value;
				} else if (typeString == "string16") {
					eastl::string16 value;
					result = App::Property::GetString16(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					string16ValueMapOverride[replaceHash] = value;
				} else if (typeString == "key") {
					ResourceKey value;
					result = App::Property::GetKey(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					keyValueMapOverride[replaceHash] = value;
				} else if (typeString == "text") {
					LocalizedString value;
					result = App::Property::GetText(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					textValueMapOverride[replaceHash] = value;
				} else if (typeString == "vector2") {
					Vector2 value;
					result = App::Property::GetVector2(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					vector2ValueMapOverride[replaceHash] = value;
				} else if (typeString == "vector3") {
					Vector3 value;
					result = App::Property::GetVector3(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					vector3ValueMapOverride[replaceHash] = value;
				} else if (typeString == "vector4") {
					Vector4 value;
					result = App::Property::GetVector4(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					vector4ValueMapOverride[replaceHash] = value;
				} else if (typeString == "colorrgb") {
					ColorRGB value;
					result = App::Property::GetColorRGB(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					colorRGBValueMapOverride[replaceHash] = value;
				} else if (typeString == "colorrgba") {
					ColorRGBA value;
					result = App::Property::GetColorRGBA(propList.get(), id(valueString.c_str()), value);
					if (!result) continue;
					colorRGBAValueMapOverride[replaceHash] = value;
				} else if (typeString == "transform") {
					Transform* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueTransform();
					transformValueMapArrayOverride[replaceHash] = eastl::pair<Transform*, uint32_t> {value, 1};
				} else if (typeString == "bbox") {
					BoundingBox* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueBBox();
					bBoxValueMapArrayOverride[replaceHash] = eastl::pair<BoundingBox*, uint32_t> {value, 1};
				} else if (typeString == "bools") {
					bool* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueBool();
					size = out.ext->GetItemCount();
					boolValueMapArrayOverride[replaceHash] = eastl::pair<bool*, uint32_t>{value, size};
				} else if (typeString == "int32s") {
					int32_t* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueInt32();
					size = out.ext->GetItemCount();
					int32ValueMapArrayOverride[replaceHash] = eastl::pair<int32_t*, uint32_t>{value, size};
				} else if (typeString == "uint32s") {
					uint32_t* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueUInt32();
					size = out.ext->GetItemCount();
					uint32ValueMapArrayOverride[replaceHash] = eastl::pair<uint32_t*, uint32_t>{value, size};
				} else if (typeString == "floats") {
					float* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueFloat();
					size = out.ext->GetItemCount();
					floatValueMapArrayOverride[replaceHash] = eastl::pair<float*, uint32_t>{value, size};
				} else if (typeString == "string8s") {
					eastl::string* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueString8();
					size = out.ext->GetItemCount();
					string8ValueMapArrayOverride[replaceHash] = eastl::pair<eastl::string*, uint32_t>{value, size};
				} else if (typeString == "string16s") {
					eastl::string16* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueString16();
					size = out.ext->GetItemCount();
					string16ValueMapArrayOverride[replaceHash] = eastl::pair<eastl::string16*, uint32_t>{value, size};
				} else if (typeString == "keys") {
					ResourceKey* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueKey();
					size = out.ext->GetItemCount();
					keyValueMapArrayOverride[replaceHash] = eastl::pair<ResourceKey*, uint32_t>{value, size};
				} else if (typeString == "texts") {
					LocalizedString* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueText();
					size = out.ext->GetItemCount();
					textValueMapArrayOverride[replaceHash] = eastl::pair<LocalizedString*, uint32_t>{value, size};
				} else if (typeString == "vector2s") {
					Vector2* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueVector2();
					size = out.ext->GetItemCount();
					vector2ValueMapArrayOverride[replaceHash] = eastl::pair<Vector2*, uint32_t>{value, size};
				} else if (typeString == "vector3s") {
					Vector3* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueVector3();
					size = out.ext->GetItemCount();
					vector3ValueMapArrayOverride[replaceHash] = eastl::pair<Vector3*, uint32_t>{value, size};
				} else if (typeString == "vector4s") {
					Vector4* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueVector4();
					size = out.ext->GetItemCount();
					vector4ValueMapArrayOverride[replaceHash] = eastl::pair<Vector4*, uint32_t>{value, size};
				} else if (typeString == "colorrgbs") {
					ColorRGB* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueColorRGB();
					size = out.ext->GetItemCount();
					colorRGBValueMapArrayOverride[replaceHash] = eastl::pair<ColorRGB*, uint32_t>{value, size};
				} else if (typeString == "colorrgbas") {
					ColorRGBA* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueColorRGBA();
					size = out.ext->GetItemCount();
					colorRGBAValueMapArrayOverride[replaceHash] = eastl::pair<ColorRGBA*, uint32_t>{value, size};
				} else if (typeString == "transforms") {
					Transform* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueTransform();
					size = out.ext->GetItemCount();
					transformValueMapArrayOverride[replaceHash] = eastl::pair<Transform*, uint32_t>{value, size};
				} else if (typeString == "bboxs") {
					BoundingBox* value;
					result = propList->GetProperty(id(valueString.c_str()), out.prop);
					if (!result) continue;
					value = out.ext->GetValueBBox();
					size = out.ext->GetItemCount();
					bBoxValueMapArrayOverride[replaceHash] = eastl::pair<BoundingBox*, uint32_t>{value, size};
				}
			}
		}
		return true;
	}
}

using namespace UniversalPropertyReplacement;

template<typename T>
void ApplyTemplateValueMapProperty(App::Property*& prop, uint32_t propertyID, eastl::map<uint32_t, T>& map) {
	auto iter = map.find(propertyID);
	if (iter != map.end()) {
		prop->Set(prop->mnType, 0, (void*)&(iter->second), sizeof(T), 1);
	}
}

template<typename T>
void ApplyTemplateValueMapArrayProperty(App::Property*& prop, uint32_t propertyID, eastl::map<uint32_t, eastl::pair<T*, uint32_t>>& map) {
	auto iter = map.find(propertyID);
	if (iter != map.end()) {
		prop->Set(prop->mnType, App::Property::PropertyFlags::kPropertyFlagArray, (void*)(iter->second.first), sizeof(T), iter->second.second);
	}
}

void ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID) {
	if (deletePropertyIDs.find(propertyID) != deletePropertyIDs.end()) {
		prop->Set(App::PropertyType::Void, 0, NULL, 0, 0);
		return;
	}
	if (prop->mnFlags & App::Property::PropertyFlags::kPropertyFlagArray) {
		switch (prop->mnType) {
			case App::PropertyType::Bool: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, boolValueMapArrayOverride);
			}
			case App::PropertyType::Int32: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, int32ValueMapArrayOverride);
			}
			case App::PropertyType::UInt32: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, uint32ValueMapArrayOverride);
			}
			case App::PropertyType::Float: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, floatValueMapArrayOverride);
			}
			case App::PropertyType::String8: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, string8ValueMapArrayOverride);
			}
			case App::PropertyType::String16: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, string16ValueMapArrayOverride);
			}
			case App::PropertyType::Key: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, keyValueMapArrayOverride);
			}
			case App::PropertyType::Text: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, textValueMapArrayOverride);
			}
			case App::PropertyType::Vector2: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, vector2ValueMapArrayOverride);
			}
			case App::PropertyType::Vector3: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, vector3ValueMapArrayOverride);
			}
			case App::PropertyType::Vector4: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, vector4ValueMapArrayOverride);
			}
			case App::PropertyType::ColorRGB: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, colorRGBValueMapArrayOverride);
			}
			case App::PropertyType::ColorRGBA: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, colorRGBAValueMapArrayOverride);
			}
			case App::PropertyType::Transform: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, transformValueMapArrayOverride);
			}
			case App::PropertyType::BBox: {
				return ApplyTemplateValueMapArrayProperty(prop, propertyID, bBoxValueMapArrayOverride);
			}
		}
		return;
	}
	switch (prop->mnType) {
		case App::PropertyType::Bool: {
			return ApplyTemplateValueMapProperty(prop, propertyID, boolValueMapOverride);
		}
		case App::PropertyType::Int32: {
			return ApplyTemplateValueMapProperty(prop, propertyID, int32ValueMapOverride);
		}
		case App::PropertyType::UInt32: {
			return ApplyTemplateValueMapProperty(prop, propertyID, uint32ValueMapOverride);
		}
		case App::PropertyType::Float: {
			return ApplyTemplateValueMapProperty(prop, propertyID, floatValueMapOverride);
		}
		case App::PropertyType::String8: {
			return ApplyTemplateValueMapProperty(prop, propertyID, string8ValueMapOverride);
		}
		case App::PropertyType::String16: {
			return ApplyTemplateValueMapProperty(prop, propertyID, string16ValueMapOverride);
		}
		case App::PropertyType::Key: {
			return ApplyTemplateValueMapProperty(prop, propertyID, keyValueMapOverride);
		}
		case App::PropertyType::Text: {
			return ApplyTemplateValueMapProperty(prop, propertyID, textValueMapOverride);
		}
		case App::PropertyType::Vector2: {
			return ApplyTemplateValueMapProperty(prop, propertyID, vector2ValueMapOverride);
		}
		case App::PropertyType::Vector3: {
			return ApplyTemplateValueMapProperty(prop, propertyID, vector3ValueMapOverride);
		}
		case App::PropertyType::Vector4: {
			return ApplyTemplateValueMapProperty(prop, propertyID, vector4ValueMapOverride);
		}
		case App::PropertyType::ColorRGB: {
			return ApplyTemplateValueMapProperty(prop, propertyID, colorRGBValueMapOverride);
		}
		case App::PropertyType::ColorRGBA: {
			return ApplyTemplateValueMapProperty(prop, propertyID, colorRGBAValueMapOverride);
		}
		/* unreachable properties
		case App::PropertyType::Transform: {
			return ApplyTemplateValueMapProperty(prop, propertyID, transformValueMapOverride);
		}
		case App::PropertyType::BBox: {
			return ApplyTemplateValueMapProperty(prop, propertyID, bBoxValueMapOverride);
		}*/
	}
}

template<>
bool UniversalPropertyReplacement::GetPropertyAlt__detour::DETOUR(uint32_t propertyID, App::Property*& result) {

	return this->GetProperty(propertyID, result);
}

template<>
bool UniversalPropertyReplacement::GetProperty__detour::DETOUR(uint32_t propertyID, App::Property*& result) {
	const bool ret = original_function(this, propertyID, result);
	if (ret) ApplyValueMapProperty(result, propertyID);
	return ret;	
}

template<>
App::Property* UniversalPropertyReplacement::GetPropertyObject__detour::DETOUR(uint32_t propertyID) {
	App::Property* ret = original_function(this, propertyID);
	ApplyValueMapProperty(ret, propertyID);
	return ret;
}

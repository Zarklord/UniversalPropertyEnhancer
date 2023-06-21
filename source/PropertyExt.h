/****************************************************************************
* Copyright (C) 2022 Zarklord
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

class PropertyExt : public App::Property
{
public:
	size_t GetItemCount() const
	{
		return this->mnItemCount;
	}
	size_t GetItemSize() const
	{
		return this->mnItemSize;
	}
	bool IsArray() const
	{
		return (mnFlags & kPropertyFlagArray) == kPropertyFlagArray;
	}
	bool IsNotDirect() const
	{
		return (mnFlags & kPropertyFlagNotDirect) == kPropertyFlagNotDirect;
	}

	struct array_string_8
	{
		const char * mBegin;
		const char * mEnd;
	};
	
	static bool GetArrayString8(const App::PropertyList* pPropertyList, uint32_t propertyID, size_t& dstCount, array_string_8*& dst)
	{
		//as much as GetArrayString8 says it gives a string8 array it doesn't.
		return Property::GetArrayString8(pPropertyList, propertyID, dstCount, (eastl::string*&)dst);
	}
};

inline void CopyProperty(App::Property * prop, App::Property * copy_prop)
{
	memcpy(copy_prop, prop, sizeof(App::Property));
}

inline size_t GetArrayItemCount(App::Property * prop)
{
	const auto prop_ex = static_cast<PropertyExt*>(prop);
	return prop_ex->GetItemCount();
}
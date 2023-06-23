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

namespace Extensions
{
	class Property : public App::Property
	{
	public:
		operator App::Property*()
		{
			return this;
		}

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
		
		static bool Get8ByteArrayString8(const App::PropertyList* pPropertyList, uint32_t propertyID, size_t& dstCount, array_string_8*& dst)
		{
			dstCount = 0;
			dst = nullptr;

			App::Property* prop;
			if (!pPropertyList->GetProperty(propertyID, prop))
				return false;

			const auto extProp = static_cast<Property*>(prop);

			if (extProp->mnType != App::PropertyType::String8 || !extProp->IsArray() || extProp->GetItemSize() != 8)
				return false;

			dst = static_cast<array_string_8*>(extProp->GetValue());
			dstCount = extProp->GetItemCount();

			return true;
		}
	};

	inline size_t GetArrayItemCount(App::Property * prop)
	{
		const auto prop_ex = static_cast<Property*>(prop);
		return prop_ex->GetItemCount();
	}
}
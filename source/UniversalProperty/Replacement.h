/****************************************************************************
* Copyright (C) 2018, 2020, 2022 Zarklord
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
#include "PropertyExt.h"
#include <EASTL/hash_set.h>
#include "typename.h"

class UniversalPropertyReplacer
{
public:
	class ReplaceBase
	{
	public:
		virtual ~ReplaceBase() = 0;
		virtual bool LoadValueMapProperty(uint32_t type_hash, const App::PropertyList* prop_list, uint32_t value_hash, uint32_t replace_hash) = 0;
		virtual void ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID) = 0;
		virtual void Test(App::PropertyList* temp_list, string& error_str) = 0;
		App::PropertyType mPropertyType = App::PropertyType::None;
	};

	struct ReplaceBaseHash
		{ size_t operator()(const ReplaceBase* val) const { return static_cast<size_t>(val->mPropertyType); } };

	static constexpr uint32_t sUniversalPropertyReplacerTest = id("UniversalPropertyReplacer");

	template<typename T, App::PropertyType pType, App::Property::PropertyFlags pFlags, uint32_t sType, uint32_t sType2 = 0>
	class ReplaceType : public ReplaceBase
	{
	public:
		static constexpr bool is_array = pFlags == App::Property::PropertyFlags::kPropertyFlagArray;

		ReplaceType()
		{
			mPropertyType = pType;
		}

		~ReplaceType() override
		{
			valueMapOverride.clear();		
		}

		bool LoadValueMapProperty(uint32_t type_hash, const App::PropertyList* prop_list, uint32_t value_hash, uint32_t replace_hash) override
		{
			if (!(type_hash == sType || type_hash == sType2)) return false;
			
			App::Property* prop;
			if (!prop_list->GetProperty(value_hash, prop)) return false;
			
			valueMapOverride[replace_hash] = pair(prop->GetValue(), is_array ? GetArrayItemCount(prop) : 1);

			return true;
		}

		void ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID) override
		{
			if ((prop->mnFlags & pFlags) != pFlags || prop->mnType != pType) return;
			auto iter = valueMapOverride.find(propertyID);
			if (iter != valueMapOverride.end()) {
				prop->Set(pType, pFlags, iter->second.first, sizeof(T), iter->second.second);
			}
		}

		void Test(App::PropertyList* temp_list, string& error_str) override
		{
			App::Property prop{};

			constexpr uint32_t test_value_count = is_array ? 2 : 1;

			void * test_number = new char[test_value_count * sizeof(T)];
			void * replace_number = new char[test_value_count * sizeof(T)];

			//set some dummy values to distinguish the test vs replaced numbers
			memset(test_number, 't', 4);
			memset(replace_number, 'r', 4);
			
			prop.Set(pType, pFlags, test_number, sizeof(T), test_value_count );
			temp_list->SetProperty(sUniversalPropertyReplacerTest, &prop);

			valueMapOverride[sUniversalPropertyReplacerTest] = pair(replace_number, test_value_count);

			App::Property* get_prop{};
			temp_list->GetProperty(sUniversalPropertyReplacerTest, get_prop);
			App::Property *get_prop_object = temp_list->GetPropertyObject(sUniversalPropertyReplacerTest);
			
			valueMapOverride.erase(sUniversalPropertyReplacerTest);
			
			const uint32_t prop_value = *static_cast<uint32_t*>(prop.GetValue());
			const uint32_t get_prop_value = *static_cast<uint32_t*>(get_prop->GetValue());
			const uint32_t get_prop_object_value = *static_cast<uint32_t*>(get_prop_object->GetValue());

			//get_prop and get_prop_object must be the same, and different from prop
			if (prop_value == get_prop_value || get_prop_value != get_prop_object_value)
			{
				delete[] static_cast<char*>(replace_number);
				error_str += string("Failed property replacement for: ") + (is_array ? TypeName<T*>() : TypeName<T>()) + "\n";
			}

			temp_list->RemoveProperty(sUniversalPropertyReplacerTest);
		}
	private:
		map<uint32_t, pair<void*, uint32_t>> valueMapOverride;
	};

public:
	static void Initialize();
	static void Finalize();
	static UniversalPropertyReplacer& Get();
	static bool Exists();
	
	static constexpr uint32_t sGroupID = id("prop_overrides");
	static constexpr uint32_t sArgumentList = id("replacementList");

	static constexpr uint32_t sDelete = id("delete");

	static void AttachDetours();

private:
	UniversalPropertyReplacer();
public:
	~UniversalPropertyReplacer();

	void ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID);

	bool GetTestResults(string& error_string) const;
private:
	void LoadReplacementLists();

	void PostConstruct();
	void Test();

	static UniversalPropertyReplacer* mInstance;

	string8 mErrorString;
	
	hash_set<uint32_t> mDeletePropertyIDs;
	hash_set<ReplaceBase*, ReplaceBaseHash> mValueReplacements;
	hash_set<ReplaceBase*, ReplaceBaseHash> mArrayReplacements;
};
inline UniversalPropertyReplacer::ReplaceBase::~ReplaceBase() = default;

template <> struct hash<App::PropertyType>
	{ size_t operator()(App::PropertyType val) const { return static_cast<size_t>(val); } };
		
inline bool operator==(const UniversalPropertyReplacer::ReplaceBase *const& lhs, const App::PropertyType& rhs)
	{ return lhs->mPropertyType == rhs; }

inline bool operator!=(const UniversalPropertyReplacer::ReplaceBase *const& lhs, const App::PropertyType& rhs)
	{ return lhs->mPropertyType != rhs; }

UniversalPropertyReplacer& GetUniversalPropertyReplacer();
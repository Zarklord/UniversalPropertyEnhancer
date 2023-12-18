/****************************************************************************
* Copyright (C) 2018, 2020, 2022, 2023 Zarklord
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
#include <memory>
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

	template<typename T, App::PropertyType pType, bool IsArray, uint32_t sType, uint32_t sType2 = 0>
	class ReplaceType : public ReplaceBase
	{
	public:
		ReplaceType()
		{
			mPropertyType = pType;
		}

		~ReplaceType() override
		{
			for (const auto& override : valueMapOverride)
			{
				delete override.second.first;
			}
			valueMapOverride.clear();		
		}

		bool LoadValueMapProperty(uint32_t type_hash, const App::PropertyList* prop_list, uint32_t value_hash, uint32_t replace_hash) override
		{
			if (!(type_hash == sType || type_hash == sType2)) return false;
			
			App::Property* prop;
			if (!prop_list->GetProperty(value_hash, prop)) return false;

			const auto item_count = IsArray ? Extensions::GetArrayItemCount(prop) : 1;
			T* value = static_cast<T*>(prop->GetValue());
			T* new_value = new T[item_count];
			for (unsigned int i = 0; i < item_count; ++i)
				new_value[i] = value[i];
			valueMapOverride[replace_hash] = pair(new_value, item_count);

			return true;
		}

		void ApplyValueMapProperty(App::Property*& prop, uint32_t propertyID) override
		{
			const Extensions::Property* ext_prop = GetPropertyExt(prop);
			if (ext_prop->IsArray() != IsArray || prop->mnType != pType) return;
			const auto iter = valueMapOverride.find(propertyID);
			if (iter != valueMapOverride.end()) {
				constexpr short clear_flags = ~(Extensions::Property::kPropertyFlagCleanup & Extensions::Property::kPropertyFlagUnk8);
				const auto flags = static_cast<short>(static_cast<short>(prop->mnFlags & clear_flags) | Extensions::Property::kPropertyFlagSkipDealloc);
				prop->Set(pType, flags, iter->second.first, sizeof(T), iter->second.second);
			}
		}

		void Test(App::PropertyList* temp_list, string& error_str) override
		{
			App::Property prop{};

			constexpr uint32_t test_value_count = IsArray ? 2 : 1;
			const uint32_t array_size = test_value_count * sizeof(T);
			const uint32_t cmp_size = std::min(4u, array_size);

			const auto test_number = std::make_unique<char[]>(array_size);
			const auto replace_number = std::make_unique<char[]>(array_size);

			//set some dummy values to distinguish the test vs replaced numbers
			memset(test_number.get(), 't', cmp_size);
			memset(replace_number.get(), 'r', cmp_size);

			const short flags = (IsArray ? Extensions::Property::kPropertyFlagPointer : Extensions::Property::kPropertyFlagNone) | Extensions::Property::kPropertyFlagSkipDealloc;
			prop.Set(pType, flags, test_number.get(), sizeof(T), test_value_count);
			temp_list->SetProperty(sUniversalPropertyReplacerTest, &prop);

			valueMapOverride[sUniversalPropertyReplacerTest] = pair(reinterpret_cast<T*>(replace_number.get()), test_value_count);

			App::Property* get_prop{};
			temp_list->GetProperty(sUniversalPropertyReplacerTest, get_prop);
			App::Property *get_prop_object = temp_list->GetPropertyObject(sUniversalPropertyReplacerTest);
			
			valueMapOverride.erase(sUniversalPropertyReplacerTest);
			
			const void* prop_value = prop.GetValue();
			const void* get_prop_value = get_prop->GetValue();
			const void* get_prop_object_value = get_prop_object->GetValue();

			//get_prop and get_prop_object must be the same, and different from prop
			if (memcmp(prop_value, get_prop_value, cmp_size) == 0 || memcmp(get_prop_value, get_prop_object_value, cmp_size) != 0)
			{
				const eastl::string failure = string("Failed property replacement for: ") + (IsArray ? TypeName<T*>() : TypeName<T>());
				ModAPI::Log(failure.c_str());
				error_str += failure + "\n";
			}

			temp_list->RemoveProperty(sUniversalPropertyReplacerTest);
		}
	private:
		map<uint32_t, pair<T*, uint32_t>> valueMapOverride;
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
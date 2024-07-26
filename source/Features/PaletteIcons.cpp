/****************************************************************************
* Copyright (C) 2018-2024 Zarklord
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

#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

#include <Spore/Resource/cResourceManager.h>

static sol::function sGetPaletteIcons;

struct PaletteIcon {
	uint32_t id;
	int32_t sequence;
	uint32_t layout_id;
	ResourceKey icon;
	ResourceKey bgicon;
	LocalizedString name;
};

class PaletteIcons {
	public:
		vector<PaletteIcon*> mPaletteIcons;
};

OnLuaInit(sol::state_view s, bool is_main_state)
{
	s.new_usertype<PaletteIcon>(
		"PaletteIcon",
		sol::call_constructor, sol::initializers(
			[](PaletteIcon* memory, const LuaFNVHash& id)
			{
				auto* self = new(memory) PaletteIcon();
				self->id = id;
				self->layout_id = 0x98F661C7;
			}
		),
		"SetSequence", [](PaletteIcon& icon, int32_t sequence)
		{
			icon.sequence = sequence;
		},
		"SetLayoutID", [](PaletteIcon& icon, const LuaFNVHash& layout_id)
		{
			icon.layout_id = layout_id;
		},
		"SetIcon", [](PaletteIcon& icon, const ResourceKey& button_icon, const ResourceKey& button_bgicon)
		{
			icon.icon = button_icon;
			icon.bgicon = button_bgicon;
		},
		"SetName", sol::overload(
			[](PaletteIcon& icon, const LocalizedString& name)
			{
				icon.name = name;
			},
			[](PaletteIcon& icon, const LuaFNVHash& tableID, const LuaFNVHash& instanceID, sol::optional<const char16_t*> text)
			{
				icon.name.SetText(tableID, instanceID, text.value_or(nullptr));
			},
			[](PaletteIcon& icon, const App::Property::TextProperty& text)
			{
				icon.name.SetText(text.tableID, text.instanceID);
			}
		)
	);

	if (!is_main_state) return;

	s["AddCustomPaletteIcons"] = [](const sol::function& fn)
	{
		sGetPaletteIcons = fn;
	};
}

OnLuaDispose(sol::state_view s, bool is_main_state)
{
	if (!is_main_state) return;
	sGetPaletteIcons.reset();
}

member_detour(LoadPaletteIconProps_detour, PaletteIcons, void())
{
	void detoured()
	{
		original_function(this);
		
		if (LuaSpore::CanExecuteOnMainState() && sGetPaletteIcons)
		{
			const sol::table palette_icons = sGetPaletteIcons();
			const size_t palette_icons_size = palette_icons.size();

			mPaletteIcons.reserve(mPaletteIcons.size() + palette_icons_size);
			palette_icons.for_each([this](sol::object key, const sol::object& value)
			{
				mPaletteIcons.push_back(new PaletteIcon{value.as<PaletteIcon&>()});
			});

			std::sort(mPaletteIcons.begin(), mPaletteIcons.end(), [](const PaletteIcon* a, const PaletteIcon* b)
			{
				return a->sequence < b->sequence;
			});
		}
	}
};

AddSporeDetours()
{
	LoadPaletteIconProps_detour::attach(Address(ModAPI::ChooseAddress(0x7DBBA0, 0x7DB710)));
}
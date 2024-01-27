#include "pch.h"

#include "LuaSpore/LuaSporeCallbacks.h"
#include "LuaSpore/SporeDetours.h"

#include <Spore/Resource/cResourceManager.h>

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

member_detour(LoadPaletteIconProps_detour, PaletteIcons, void())
{
	void detoured()
	{
		original_function(this);
		
		LUA_THREAD_SAFETY();
		if (sGetPaletteIcons.valid())
		{
			const sol::table palette_icons = sGetPaletteIcons();
			const size_t palette_icons_size = palette_icons.size();

			mPaletteIcons.reserve(mPaletteIcons.size() + palette_icons_size);
			
			palette_icons.for_each([this](sol::object key, sol::object value)
			{
				mPaletteIcons.push_back(new PaletteIcon{value.as<PaletteIcon&>()});
			});

			std::sort(mPaletteIcons.begin(), mPaletteIcons.end(), [](const PaletteIcon* a, const PaletteIcon* b)
			{
				return a->sequence < b->sequence;
			});
		}
	}

	static inline sol::function sGetPaletteIcons;
};

OnLuaInit(sol::state_view s)
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

	s["AddCustomPaletteIcons"] = [](const sol::function& fn)
	{
		LoadPaletteIconProps_detour::sGetPaletteIcons = fn;
	};
}

OnLuaDispose(sol::state_view s)
{
	LoadPaletteIconProps_detour::sGetPaletteIcons.reset();
}

AddSporeDetours()
{
	LoadPaletteIconProps_detour::attach(Address(ModAPI::ChooseAddress(0x7DBBA0, 0x7DB710)));
}
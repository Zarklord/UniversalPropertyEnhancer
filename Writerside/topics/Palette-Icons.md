# Palette Icons
<link-summary>Use UPEs lua api to add custom palette icons to your part pages</link-summary>
<web-summary>Use UPEs lua api to add custom palette icons to your part pages</web-summary>
<card-summary>Use UPEs lua api to add custom palette icons to your part pages</card-summary>

## Feature

You can add custom palette icons with this feature, like Creepy & Cute and Bot Parts do.

## API

<include from="Snippets.topic" element-id="luaapi-wait-for-propmanager">
  <var name="upe-api-type" value="palette icon"/>
  <var name="upe-api-folder" value="paletteicons"/>
</include>

Palette Icons exposes the following API:

<deflist>
  <def title="AddPaletteIcon(palette_icon)">
    Adds a new palette icon to the list of palette icons.
    <deflist type="medium">
      <def title="palette_icon">
        <code>PaletteIcon</code> required
      </def>
    </deflist>
  </def>
</deflist>

And binds the `PaletteIcon` C++ object:

<deflist>
  <def title="PaletteIcon(id)">
    Constructs a new palette icon with an id of `id` and a layout of `0x98F661C7`.
    <deflist type="medium">
      <def title="id">
        <code>FNVHash</code> required
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetSequence(sequence)">
    Sets the sequence of a palette icon.
    This controls the order in which your palette icon appears relative to other palette icons.
    <deflist type="medium">
      <def title="sequence">
        <code>integer</code> required
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetLayoutID(layout_id)">
    Sets the layout id of a palette icon.
    Usually, you don't need to change this.
    <deflist type="medium">
      <def title="layout_id">
        <code>FNVHash</code> required
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetIcon(button_icon, button_bgicon)">
    Sets the icon of a palette icon.
    <deflist type="medium">
      <def title="button_icon">
        <code>ResourceKey</code> required
      </def>
      <def title="button_bgicon">
        <code>ResourceKey</code> required
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetName(name)">
    Sets the name (visible on hovering) of the palette icon with a <code>LocalizedString</code>.
    <deflist type="medium">
      <def title="name">
        <code>LocalizedString</code> required
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetName(tableID, instanceID, fallback_name)">
    Sets the name (visible on hovering) of the palette icon with a lookup into the locale strings.
    <deflist type="medium">
      <def title="tableID">
        <code>FNVHash</code> required
      </def>
      <def title="instanceID">
        <code>FNVHash</code> required
      </def>
      <def title="fallback_name">
        <code>string</code> optional - only used if the provided <code>tableID</code> and <code>instanceID</code> can't find a valid locale string.
      </def>
    </deflist>
  </def>
  <def title="PaletteIcon:SetName(text_property)">
    Sets the name (visible on hovering) of the palette icon from a <code>App::Property::TextProperty</code> stored in a <code>Property</code> object.
    <deflist type="medium">
      <def title="text_property">
        <code>App::Property::TextProperty</code> required
      </def>
    </deflist>
  </def>
</deflist>

## Usage

The following is an example of how you might add a custom palette icon:

Create our new palette icon:
```
local palette_icon = PaletteIcon("MyCoolMod")
```
Set the sequence:
```
palette_icon:SetSequence(1025)
```
Set the icon:
```
palette_icon:SetIcon(ResourceKey(0x011C0BDE, "MyCoolModIcon", "png"),
                     ResourceKey(0x011C0BDE, "MyCoolModBGIcon", "png"))
```
this makes the game look at `common~!mycoolmodicon.png` and `common~!mycoolmodbgicon.png` for the icons.

Set the name of the icon:
```
palette_icon:SetName("MyCoolMod", "IconName", "MyCoolIcon")
```
This looks for a `mycoolmod.locale` with an entry `0xC3CDE5FF` (hash of `IconName`) to use for the name of this palette_icon, if this entry doesn't exist, it will fallback to "MyCoolIcon" as the name.

You can directly set the name: `palette_icon:SetName("MyCoolIcon")`, but this prevents people from localizing your mod.

You can also read the name from a `TextProperty` to set the name: `palette_icon:SetName(prop:Get())`, where prop contains a text property.

And finally add it to the game:
```
AddPaletteIcon(palette_icon)
```

Lastly add this property to any page you want to use your icon:
```
int32 palettePageSetId hash(MyCoolMod)
```

<seealso style="cards">
    <category ref="external">
        <a href="https://zarklord.github.io/spore/Universal-Property-Enhancer.html" summary="Mod Page">Universal Property Enhancer</a>
        <a href="https://zarklord.github.io/Spore-LuaAPI/documentation.html" summary="Documentation">Spore LuaAPI</a>
    </category>
</seealso>
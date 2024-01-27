local search_group_id = fnv_id("paletteicons")

local palette_sequence_number_hash = 0x5D1A718;
local palette_layout_hash = 0x05DFEF47;
local palette_icon_hash = 0x05E4DE4E;
local palette_bgicon_hash = 0x05DFEF48;
local palette_name_hash = 0xFE1E2BC2;

local palette_icons = {}

local function ProcessPropertyList(instance_id, prop_list)
	local palette_icon = PaletteIcon(instance_id)

	local sequence_number_prop = prop_list:GetProperty(palette_sequence_number_hash)
	local layout_prop = prop_list:GetProperty(palette_layout_hash)
	local icon_prop = prop_list:GetProperty(palette_icon_hash)
	local bgicon_prop = prop_list:GetProperty(palette_bgicon_hash)
	local name_prop = prop_list:GetProperty(palette_name_hash)

	if not icon_prop or not bgicon_prop or not sequence_number_prop then
		return
	end

	palette_icon:SetIcon(icon_prop:Get(), bgicon_prop:Get())
	palette_icon:SetSequence(sequence_number_prop:Get())
	if layout_prop then
		palette_icon:SetLayoutID(layout_prop:Get().instance_id)
	end
	if name_prop then
		palette_icon:SetName(name_prop:Get())
	end

	table.insert(palette_icons, palette_icon)
end

for i, instance_id in ipairs(PropManager:GetPropertyListIds(search_group_id)) do
	ProcessPropertyList(instance_id, PropManager:GetPropertyList(instance_id, search_group_id))
end

AddCustomPaletteIcons(function()
	return palette_icons
end)
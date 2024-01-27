local search_group_id = fnv_id("prop_overrides")
local argument_list = fnv_id("replacementList")

local type_name_to_property_type =
{
	["bool"] = PropertyType.Bool,
	["bools"] = PropertyType.Bool,
	["int32"] = PropertyType.Int32,
	["int32s"] = PropertyType.Int32,
	["uint32"] = PropertyType.UInt32,
	["uint32s"] = PropertyType.UInt32,
	["float"] = PropertyType.Float,
	["floats"] = PropertyType.Float,
	["string8"] = PropertyType.String8,
	["string8s"] = PropertyType.String8,
	["string16"] = PropertyType.String16,
	["string16s"] = PropertyType.String16,
	["key"] = PropertyType.Key,
	["keys"] = PropertyType.Key,
	["text"] = PropertyType.Text,
	["texts"] = PropertyType.Text,
	["vector2"] = PropertyType.Vector2,
	["vector2s"] = PropertyType.Vector2,
	["vector3"] = PropertyType.Vector3,
	["vector3s"] = PropertyType.Vector3,
	["vector4"] = PropertyType.Vector4,
	["vector4s"] = PropertyType.Vector4,
	["colorrgb"] = PropertyType.ColorRGB,
	["colorrgbs"] = PropertyType.ColorRGB,
	["colorrgba"] = PropertyType.ColorRGBA,
	["colorrgbas"] = PropertyType.ColorRGBA,
	["transform"] = PropertyType.Transform,
	["transforms"] = PropertyType.Transform,
	["bbox"] = PropertyType.BBox,
	["bboxs"] = PropertyType.BBox,
	["delete"] = "delete",
}

local replacement_map = {universal = {}}

local function DeleteProperty(property)
	property:SetType(PropertyType.Void)
end

local function ProcessArgument(arguments, prop_list)
	local valid_arguments = {}
	for i, v in ipairs(arguments) do
		if v:find("-", 1, true) ~= 1 then
			table.insert(valid_arguments, v)
		end
	end
	if #valid_arguments < 2 or #valid_arguments > 3 then
		return
	end

	local property_type = type_name_to_property_type[arguments[1]]
	local replace_str = arguments[2]:gsub("0[xX]", "")
	local replace_hash = tonumber(replace_str, 16)
	if not replace_hash then
		replace_hash = tonumber(replace_str:sub(1, #replace_str-1), 16)
		if not replace_hash then
			return
		end
	end
	local value_string = arguments[3]

	if property_type == "delete" then
		replacement_map.universal[replace_hash] = DeleteProperty
	end

	local value_property = prop_list:GetProperty(fnv_id(value_string))
	if not value_property then
		value_property = prop_list:GetProperty(fnv_id(value_string:sub(1, #value_string-1)))
		if not value_property then
			return
		end
	end

	if value_property:GetPropertyType() ~= property_type then
		return
	end

	local prop = Property(property_type)
	prop:CopyFrom(value_property)

	replacement_map[property_type] = replacement_map[property_type] or {}
	replacement_map[property_type][replace_hash] = prop
end

for i, instance_id in ipairs(PropManager:GetPropertyListIds(search_group_id)) do
	local prop_list = PropManager:GetPropertyList(instance_id, search_group_id)

	local argument_prop = prop_list:GetProperty(argument_list)
	if argument_prop and argument_prop:IsArray() then
		for i = 1, #argument_prop do
			ProcessArgument(argument_prop[i]:split(" "), prop_list)
		end
	end
end

SetPropertyReplacerFunction(function(id, property, prop_list)
	local universal_replacer = replacement_map.universal[id]
	if universal_replacer then
		if type(universal_replacer) == "function" then
			universal_replacer(property, prop_list, id)
		else
			property:ReferenceFrom(universal_replacer)
		end
		return
	end

	local type_replacement_map = replacement_map[property:GetPropertyType()]
	local replacer = type_replacement_map and type_replacement_map[id]
	if replacer then
		if type(replacer) == "function" then
			replacer(property, prop_list, id)
		else
			property:ReferenceFrom(replacer)
		end
		return
	end
end)
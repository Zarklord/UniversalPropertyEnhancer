local search_group_id = fnv_id("prop_duplications")
local argument_list = fnv_id("duplicateList")

local Prop_TypeID = 0x00B1B104

local duplication_map = {}

local function ProcessArgument(arguments, prop_list)
	local valid_arguments = {}
	for i, v in ipairs(arguments) do
		if v:find("-", 1, true) ~= 1 then
			table.insert(valid_arguments, v)
		end
	end
	if #valid_arguments ~= 2 then
		return
	end

	local prop_new_key = prop_list:GetProperty(arguments[1])
	if not prop_new_key then return end
	prop_new_key = prop_new_key:Get()

	--cannot be used to replace files
	if PropManager:HasPropertyList(prop_new_key.instance_id, prop_new_key.group_id) then
		return
	end
	prop_new_key.type_id = Prop_TypeID

	local prop_source_key_arg = valid_arguments[2]
	local prop_source_key = prop_list:GetProperty(prop_source_key_arg)
	if not prop_source_key then
		--old SMFX bug that truncated the last character of a string
		--this allows the old strings created to be repacked on new version and still work
		prop_source_key = prop_list:GetProperty(prop_source_key_arg:sub(1, #prop_source_key_arg - 1))
		if not prop_source_key then return end
	end

	prop_source_key = prop_source_key:Get()
	prop_source_key.type_id = Prop_TypeID

	if not duplication_map[prop_new_key] then
		duplication_map[prop_new_key] = prop_source_key
	end
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

SetPropertyListDuplicatorFunction(function(filter)
	local prop_keys = {}
	for new_prop_key, source_prop_key in pairs(duplication_map) do
		if filter:IsValid(new_prop_key) then
			table.insert(prop_keys, source_prop_key)
		end
	end
	return prop_keys
end)
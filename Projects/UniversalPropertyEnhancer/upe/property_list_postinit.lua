local search_group_id = fnv_id("prop_postinits")
local argument_list = fnv_id("postinitList")

local postinit_new_parent = fnv_id("newParent")
local postinit_remove_properties = fnv_id("removeProperties")

local postinit_groups = {}

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

	local postinit_prop = prop_list:GetProperty(arguments[1])
	if not postinit_prop or postinit_prop:GetPropertyType() ~= PropertyType.Key then return end

	local source_prop_str = valid_arguments[2]
	local source_prop = prop_list:GetProperty(source_prop_str)
	if not source_prop or source_prop:GetPropertyType() ~= PropertyType.Key then
		--old SMFX bug that truncated the last character of a string
		--this allows the old strings created to be repacked on new version and still work
		source_prop = prop_list:GetProperty(source_prop_str:sub(1, #source_prop_str - 1))
		if not source_prop or source_prop:GetPropertyType() ~= PropertyType.Key then return end
	end

	local postinit_key = postinit_prop:Get()

	local group_instances = postinit_groups[postinit_key.group_id]
	if not group_instances then
		group_instances = {}
		postinit_groups[postinit_key.group_id] = group_instances
	end

	local instance_postinits = group_instances[postinit_key.instance_id]
	if not instance_postinits then
		instance_postinits = {source_prop:Get()}
		group_instances[postinit_key.instance_id] = instance_postinits
	else
		table.insert(instance_postinits, source_prop:Get())
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

local function ApplyPropertyListPostInit(source_prop_list, prop_list)
	local new_parent_prop = source_prop_list:GetProperty(postinit_new_parent)
	if new_parent_prop then
		local new_parent_key = new_parent_prop:Get()
		local new_parent_prop_list = PropManager:GetPropertyList(new_parent_key.instance_id, new_parent_key.group_id)
		if new_parent_prop_list then
			prop_list:SetParent(new_parent_prop_list)
		end
		source_prop_list:RemoveProperty(postinit_new_parent)
	end

	local remove_properties = source_prop_list:GetProperty(postinit_remove_properties)
	if remove_properties then
		for i = 1, #remove_properties do
			prop_list:RemoveProperty(remove_properties[i])
		end
		source_prop_list:RemoveProperty(postinit_remove_properties)
	end

	prop_list:AddAllPropertiesFrom(source_prop_list)
end

SetPropertyListPostInitFunction(function(prop_list, instance_id, group_id)
	local group_instances = postinit_groups[group_id]
	if not group_instances then
		return
	end

	local instance_postinits = group_instances[instance_id]
	if not instance_postinits then
		return
	end

	for i, postinit in ipairs(instance_postinits) do
		if type(postinit) == "function" then
			postinit(prop_list, instance_id, group_id)
		else
			local source_prop_list = PropManager:GetPropertyList(postinit.instance_id, postinit.group_id)
			if source_prop_list then
				ApplyPropertyListPostInit(source_prop_list, prop_list)
			end
		end
	end
end)
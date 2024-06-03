function on_key_press()
	-- print("on_key_press")
	-- trigger_event("PluginEvent", 100, 200, "a")
end

function temp(entity, component)
	print(entity:get_id())
	print(entity:has_component("TransformComponent"))
	print(component.name)
	print(entity:get_component("TransformComponent"))
	print(get_transform_position_x(entity))
	local transform_x = get_transform_position_x(entity)
	transform_x = 700
	set_transform_position_x(entity, transform_x)
	print(get_transform_position_x(entity))
end

function on_key_press()
	-- print("on_key_press")
	-- trigger_event("PluginEvent", 100, 200, "a")
end

function temp(entity, component)
	local transform_component = entity:get_component("TransformComponent")
	print(get_transform_position_y(transform_component))
	local transform_x = get_transform_position_x(transform_component)
	transform_x = 900
	set_transform_position_x(transform_component, transform_x)
	print(get_transform_position_x(transform_component))
end

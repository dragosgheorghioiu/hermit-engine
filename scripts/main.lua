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

	local box_collider_component = entity:get_component("BoxColliderComponent")
	print(get_box_collider_width(box_collider_component))

	local rigid_body_component = entity:get_component("RigidBodyComponent")
	set_rigidbody_max_velocity_y(rigid_body_component, 0)

	local animation_component = entity:get_component("AnimationComponent")
	print(get_animation_index(animation_component))
	set_animation_index(animation_component, 4)
	print(get_animation_index(animation_component))
end

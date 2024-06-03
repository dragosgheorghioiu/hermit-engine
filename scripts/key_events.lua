local key_events_module = {}

function key_events_module.on_key_press(entity, key)
	print("on_key_press")
	if entity:has_tag("player") then
		local rigid_body_component = entity:get_component("RigidBodyComponent")
		if key == 32 then
			set_rigidbody_velocity_y(rigid_body_component, -500)
			set_rigidbody_max_velocity_y(rigid_body_component, 200)
		end
	end
end

function key_events_module.test_function()
	print("test_function")
end

return key_events_module

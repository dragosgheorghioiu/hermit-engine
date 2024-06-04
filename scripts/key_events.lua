local key_events_module = {}

function key_events_module.on_key_press(entity, key)
	if entity:has_tag("player") then
		local rigid_body_component = entity:get_component("RigidBodyComponent")
		local animation_component = entity:get_component("AnimationComponent")
		if key == keyboard_input.SPACE then
			set_rigidbody_velocity_y(rigid_body_component, 0)
			set_rigidbody_acceleration_y(rigid_body_component, 0)
		end
		-- right arrow
		if key == keyboard_input.RIGHT then
			set_rigidbody_velocity_x(rigid_body_component, 200)
			set_animation_index(animation_component, 2)
		end
		-- left arrow
		if key == keyboard_input.LEFT then
			set_rigidbody_velocity_x(rigid_body_component, -200)
			set_animation_index(animation_component, 2)
			set_sprite_flip_h(entity)
		end
	end
end

return key_events_module

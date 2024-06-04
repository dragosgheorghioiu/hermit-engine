---@diagnostic disable: undefined-global
local key_events_module = {}

local player_is_looking_right = true

function key_events_module.on_key_press(entity, key)
	if entity:has_tag("player") then
		local rigid_body_component = entity:get_component("RigidBodyComponent")
		local animation_component = entity:get_component("AnimationComponent")
		local sprite_component = entity:get_component("SpriteComponent")

		if key == keyboard_input.SPACE then
			set_rigidbody_velocity_y(rigid_body_component, -600)
		end
		-- right arrow
		if key == keyboard_input.RIGHT then
			set_rigidbody_acceleration_x(rigid_body_component, 4000)
			if not player_is_looking_right then
				set_sprite_flip_h(sprite_component)
			end
			player_is_looking_right = true
		end
		-- left arrow
		if key == keyboard_input.LEFT then
			set_rigidbody_acceleration_x(rigid_body_component, -4000)
			if player_is_looking_right then
				set_sprite_flip_h(sprite_component)
			end
			player_is_looking_right = false
		end
	end
end

return key_events_module

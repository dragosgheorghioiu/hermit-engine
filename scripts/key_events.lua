---@diagnostic disable: undefined-global
local player = require("player")
local key_events_module = {}

function key_events_module.on_key_press(entity, key)
	if entity:has_tag("player") then
		local rigid_body_component = entity:get_component("RigidBodyComponent")
		local sprite_component = entity:get_component("SpriteComponent")

		if key == keyboard_input.SPACE then
			player.controls.jump = true
		end
		-- right arrow
		if key == keyboard_input.RIGHT then
			player.controls.right = true
		end
		-- left arrow
		if key == keyboard_input.LEFT then
			player.controls.left = true
		end
	end
end

function key_events_module.on_key_release(entity, key)
	if entity:has_tag("player") then
		if key == keyboard_input.SPACE then
			player.controls.jump = false
		end
		-- right arrow
		if key == keyboard_input.RIGHT then
			player.controls.right = false
		end
		-- left arrow
		if key == keyboard_input.LEFT then
			player.controls.left = false
		end
	end
end

return key_events_module

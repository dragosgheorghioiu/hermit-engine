---@diagnostic disable: undefined-global, undefined-field, lowercase-global
local key_events_module = require("key_events")
local collisions_module = require("collisons")
local player = require("player")

on_key_press = key_events_module.on_key_press
on_key_release = key_events_module.on_key_release

on_collision = collisions_module.on_collision

-- function that will be called once at the start
setup = function(registry)
	-- reset the player
	player.is_grounded = false
	player.is_looking_right = true
	player.controls = {
		left = false,
		right = false,
		jump = false,
	}
	player.default_acceleration_y = 0
	player.entity = nil
	player.components = {}
	-- get the player entity and its components
	player.entity = registry:get_entity_by_tag("player")
	player.components.rigid_body_component = player.entity:get_component("RigidBodyComponent")
	player.components.animation_component = player.entity:get_component("AnimationComponent")
	player.components.sprite_component = player.entity:get_component("SpriteComponent")
	player.default_acceleration_y = get_rigidbody_acceleration_y(player.components.rigid_body_component)
end

-- function that will be called every frame
update = function(registry, deltaTime)
	-- check if the player is not grounded
	if get_rigidbody_velocity_y(player.components.rigid_body_component) ~= 0 then
		player.is_grounded = false
	end

	-- when falling, increase the gravity
	if get_rigidbody_velocity_y(player.components.rigid_body_component) >= 5 then
		set_rigidbody_acceleration_y(
			player.components.rigid_body_component,
			get_rigidbody_acceleration_y(player.components.rigid_body_component) + 75
		)
	else
		set_rigidbody_acceleration_y(player.components.rigid_body_component, player.default_acceleration_y)
	end
	player_movement()
	player_animation()
end

function player_animation()
	if not player.is_grounded then
		set_animation_index(player.components.animation_component, 5)
	elseif player.is_grounded and get_rigidbody_velocity_x(player.components.rigid_body_component) ~= 0 then
		set_animation_index(player.components.animation_component, 2)
	else
		set_animation_index(player.components.animation_component, 1)
	end
end

function player_movement()
	if player.controls.jump then
		if player.is_grounded then
			set_rigidbody_velocity_y(player.components.rigid_body_component, -800)
		end
	end
	local direction = 0
	if player.controls.right then
		direction = direction + 1
		if not player.is_looking_right then
			player.is_looking_right = true
			set_sprite_flip_h(player.components.sprite_component)
		end
	end
	if player.controls.left then
		direction = direction - 1
		if player.is_looking_right then
			player.is_looking_right = false
			set_sprite_flip_h(player.components.sprite_component)
		end
	end

	-- if no direction is pressed, stop the player slowly
	if direction == 0 then
		local velocity_x = get_rigidbody_velocity_x(player.components.rigid_body_component)
		if velocity_x ~= 0 then
			local acceleration_x = -2500.0 * velocity_x / math.abs(velocity_x)
			if math.abs(velocity_x) < 100 then
				set_rigidbody_velocity_x(player.components.rigid_body_component, 0)
				set_rigidbody_acceleration_x(player.components.rigid_body_component, 0)
				return
			else
				set_rigidbody_acceleration_x(player.components.rigid_body_component, acceleration_x)
			end
		end
		return
	end

	-- calculate final acceleration
	local acceleration_x = 3000.0 * direction
	local velocity_x = get_rigidbody_velocity_x(player.components.rigid_body_component)
	if acceleration_x * velocity_x < 0 then
		acceleration_x = acceleration_x * 1.7
	end
	set_rigidbody_acceleration_x(player.components.rigid_body_component, acceleration_x)
end

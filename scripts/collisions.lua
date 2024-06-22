---@diagnostic disable: undefined-global

local utils = require("utils.collider")
local lua_player = require("player")

collisions = {}
collisions.on_collision = function(entity1, entity2)
	if entity1:has_tag("player") and entity2:belongs_to_group("walls") then
		collisions.handle_player_wall_collision(entity1, entity2)
	elseif entity2:has_tag("player") and entity1:belongs_to_group("walls") then
		collisions.handle_player_wall_collision(entity2, entity1)
	elseif entity1:belongs_to_group("shadow") and entity2:belongs_to_group("walls") then
		collisions.handle_player_wall_collision(entity1, entity2)
	elseif entity2:belongs_to_group("shadow") and entity1:belongs_to_group("walls") then
		collisions.handle_player_wall_collision(entity2, entity1)
	end
end

collisions.handle_player_wall_collision = function(player, wall)
	local player_transform_component = player:get_component("TransformComponent")
	local player_box_collider_component = player:get_component("BoxColliderComponent")

	-- get actual player min and max
	local player_pos = utils.box_collider_limit_from_transform(player)
	local player_min = { player_pos[1], player_pos[2] }
	local player_max = { player_pos[3], player_pos[4] }

	-- get actual wall min and max
	local wall_pos = utils.box_collider_limit_from_transform(wall)
	local wall_min = { wall_pos[1], wall_pos[2] }
	local wall_max = { wall_pos[3], wall_pos[4] }

	local mtv = collisions.get_mtv(player_min, player_max, wall_min, wall_max)

	player_min = { player_min[1] + mtv[1], player_min[2] + mtv[2] }

	if mtv[2] < 0 then
		set_rigidbody_velocity_y(player:get_component("RigidBodyComponent"), 0)
		lua_player.is_grounded = true
	elseif mtv[2] > 0 then
		set_rigidbody_velocity_y(player:get_component("RigidBodyComponent"), 0)
	end

	if mtv[1] ~= 0 then
		set_rigidbody_velocity_x(player:get_component("RigidBodyComponent"), 0)
	end

	set_transform_position_x(
		player_transform_component,
		player_min[1]
			- get_box_collider_position_x(player_box_collider_component)
				* get_transform_scale_x(player_transform_component)
	)
	set_transform_position_y(
		player_transform_component,
		player_min[2]
			- get_box_collider_position_y(player_box_collider_component)
				* get_transform_scale_y(player_transform_component)
	)
end

collisions.get_mtv = function(minA, maxA, minB, maxB)
	local overlapX = math.min(maxA[1] - minB[1], maxB[1] - minA[1])
	local overlapY = math.min(maxA[2] - minB[2], maxB[2] - minA[2])

	if overlapX < 0 or overlapY < 0 then
		return { 0, 0 }
	end
	if overlapX < overlapY then
		if minA[1] < minB[1] then
			return { -overlapX, 0 }
		else
			return { overlapX, 0 }
		end
	else
		if minA[2] < minB[2] then
			return { 0, -overlapY }
		end
		return { 0, overlapY }
	end
end

return collisions

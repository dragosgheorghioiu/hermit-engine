---@diagnostic disable: undefined-global, undefined-field, lowercase-global
-- import necessary modules
local key_events_module = require("key_events")
local collisions_module = require("collisions")
local player = require("player")

-- manage keyboard events
on_key_press = key_events_module.on_key_press
on_key_release = key_events_module.on_key_release

-- manage collision events
on_collision = collisions_module.on_collision

-- function that will be called once at the start
setup = function(registry)
	player.player_setup(registry)
end

-- function that will be called every frame
update = function(registry, deltaTime)
	player.player_movement()
	player.player_animation()
end

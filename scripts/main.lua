---@diagnostic disable: undefined-global
local key_events_module = require("key_events")
local collisions_module = require("collisons")

on_key_press = key_events_module.on_key_press

on_collision = collisions_module.on_collision

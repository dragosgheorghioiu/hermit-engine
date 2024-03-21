local systemhour = os.date("*t").hour

local mapTextureId = "tilemap"

if systemhour >= 22 or systemhour <= 6 then
	mapTextureId = "tilemap-night"
end

Level = {
	-- assets table for init
	assets = {
		-- textures
		[0] = { type = "texture", id = "tilemap", file = "../assets/tilemaps/jungle.png" },
		{ type = "texture", id = "tilemap-night", file = "../assets/tilemaps/jungle-night.png" },
		{ type = "texture", id = "tank", file = "../assets/images/tank-panther-right.png" },
		{ type = "texture", id = "truck", file = "../assets/images/truck-ford-right.png" },
		{ type = "texture", id = "chopper", file = "../assets/images/chopper-spritesheet.png" },
		{ type = "texture", id = "radar", file = "../assets/images/radar.png" },
		{ type = "texture", id = "projectile", file = "../assets/images/bullet.png" },
		{ type = "texture", id = "tree", file = "../assets/images/tree.png" },
		-- fonts
		{ type = "font", id = "bigblue48", file = "../assets/fonts/bigblue.ttf", fontSize = 48 },
		{ type = "font", id = "bigblue12", file = "../assets/fonts/bigblue.ttf", fontSize = 12 },
	},

	-- map table for init
	tilemap = {
		map = "../assets/tilemaps/jungle.map",
		texture = mapTextureId,
		scale = 3,
		tileSize = 32,
		mapCols = 25,
		mapRows = 20,
	},

	-- entities table for init
	entities = {
		[0] = {
			tag = "player",
			components = {
				transform = { position = { x = 20, y = 200 }, scale = { x = 2, y = 2 }, rotation = 0 },
				rigidbody = { velocity = { x = 0, y = 0 } },
				sprite = {
					width = 32,
					height = 32,
					id = "chopper",
					zIndex = 3,
					isFixed = false,
					srcRect = { x = 0, y = 32 },
				},
				animation = { frames = 2, speed = 15, loop = true },
				keyboardcontrol = {
					up = { x = 0, y = -300 },
					right = { x = 300, y = 0 },
					down = { x = 0, y = 300 },
					left = { x = -300, y = 0 },
				},
				camerafollow = {},
				health = { max = 5 },
				boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 32, y = 32 } },
				projectileemitter = {
					speed = 400,
					repeattime = 0,
					duration = 1000,
					isFriendly = true,
					damage = 1,
					angle = math.pi / 2,
				},
			},
		},
		{
			components = {
				transform = { position = { x = 1800, y = 10 }, scale = { x = 1.5, y = 1.5 }, rotation = 0 },
				rigidbody = { velocity = { x = 0, y = 0 } },
				sprite = { width = 64, height = 64, id = "radar", zIndex = 100, isFixed = true },
				animation = { frames = 8, speed = 10, loop = true },
			},
		},
		{
			tag = "label",
			components = {
				transform = { position = { x = 750, y = 10 }, scale = { x = 1, y = 1 }, rotation = 0 },
				textlabel = {
					text = "CHOPPA",
					color = { r = 255, g = 255, b = 255, a = 255 },
					font = "bigblue48",
					isFixed = false,
				},
			},
		},
		{
			group = "enemy",
			components = {
				transform = { position = { x = 500, y = 730 }, scale = { x = 2, y = 2 }, rotation = 0 },
				rigidbody = { velocity = { x = 150, y = 0 } },
				sprite = { width = 32, height = 32, id = "tank", zIndex = 3 },
				boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 32, y = 32 } },
				health = { max = 4 },
			},
		},
		{
			group = "obstacles",
			components = {
				transform = { position = { x = 400, y = 730 }, scale = { x = 2, y = 2 }, rotation = 0 },
				sprite = { width = 16, height = 32, id = "tree", zIndex = 3 },
				boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 16, y = 32 } },
			},
		},
		{
			group = "obstacles",
			components = {
				transform = { position = { x = 800, y = 730 }, scale = { x = 2, y = 2 }, rotation = 0 },
				sprite = { width = 16, height = 32, id = "tree", zIndex = 3 },
				boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 16, y = 32 } },
			},
		},
		{
			group = "enemy",
			components = {
				transform = { position = { x = 750, y = 500 }, scale = { x = 4, y = 4 }, rotation = 0 },
				rigidbody = { velocity = { x = 0, y = 0 } },
				sprite = { width = 32, height = 32, id = "truck", zIndex = 2 },
				boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 32, y = 32 } },
				health = { max = 20 },
				projectileemitter = {
					speed = 1000,
					repeattime = 400,
					duration = 1000,
					isFriendly = false,
					damage = 1,
					angle = math.pi / 2,
				},
			},
			on_update = {
				[0] = function(entity, delta_time, ellapsed_time)
					-- go in a sine wave
					local new_x = ellapsed_time * 0.1
					local new_y = 500 + math.sin(new_x * 0.1) * 100
					entity.set_position(entity, new_x, new_y)
				end,
			},
		},
	},
}

map_width = Level.tilemap.mapCols * Level.tilemap.tileSize * Level.tilemap.scale
map_height = Level.tilemap.mapRows * Level.tilemap.tileSize * Level.tilemap.scale

Level = {
	-- assets table for init
	assets = {
		-- textures
		[0] = { type = "texture", id = "tilemap", file = "assets/tilemaps/jungle.png" },
		{ type = "texture", id = "tank", file = "assets/images/tank-panther-right.png" },
		{ type = "texture", id = "truck", file = "assets/images/truck-ford-right.png" },
		{ type = "texture", id = "chopper", file = "assets/images/chopper-spritesheet.png" },
		{ type = "texture", id = "radar", file = "assets/images/radar.png" },
		{ type = "texture", id = "projectile", file = "assets/images/bullet.png" },
		{ type = "texture", id = "tree", file = "assets/images/tree.png" },
		-- fonts
		{ type = "font", id = "bigblue48", file = "assets/fonts/bigblue.ttf", fontSize = 48 },
		{ type = "font", id = "bigblue12", file = "assets/fonts/bigblue.ttf", fontSize = 12 },
	},

	-- map table for init
	tilemap = {
		map = "assets/tilemaps/jungle.map",
		texture = "tilemap",
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
		-- {
		-- 	group = "enemy",
		-- 	components = {
		-- 		transform = { position = { x = 750, y = 500 }, scale = { x = 4, y = 4 }, rotation = 0 },
		-- 		rigidbody = { velocity = { x = 50, y = 0 } },
		-- 		sprite = { width = 32, height = 32, id = "truck", zIndex = 2 },
		-- 		boxcollider = { offset = { x = 0, y = 0 }, dimensions = { x = 32, y = 32 } },
		-- 		health = { max = 20 },
		-- 		projectileemitter = {
		-- 			offset = { x = 75, y = 75 },
		-- 			speed = 1000,
		-- 			repeattime = 4000,
		-- 			duration = 1000,
		-- 			isFriendly = false,
		-- 			damage = 1,
		-- 		},
		-- 	},
		-- },
	},
}

--
-- //   Entity tank = registry->CreateEntity();
-- //   tank.Group("enemy");
-- //   tank.AddComponent<HealthComponent>(4);
-- //   tank.AddComponent<TransformComponent>(glm::vec2(500, 730),
-- //                                         glm::vec2(2.0, 2.0));
-- //   tank.AddComponent<RigidBodyComponent>(glm::vec2(150.0, 0));
-- //   tank.AddComponent<SpriteComponent>(32, 32, "tank", 3);
-- //   tank.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32,
-- //   32));
-- //
-- //   Entity treeA = registry->CreateEntity();
-- //   treeA.Group("obstacles");
-- //   treeA.AddComponent<TransformComponent>(glm::vec2(400, 730),
-- //                                          glm::vec2(2.0, 2.0));
-- //   treeA.AddComponent<SpriteComponent>(16, 32, "tree", 3);
-- //   treeA.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(16,
-- //   32));
-- //
-- //   Entity treeB = registry->CreateEntity();
-- //   treeB.Group("obstacles");
-- //   treeB.AddComponent<TransformComponent>(glm::vec2(800, 730),
-- //                                          glm::vec2(2.0, 2.0));
-- //   treeB.AddComponent<SpriteComponent>(16, 32, "tree", 3);
-- //   treeB.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(16,
-- //   32));
-- //
-- //   // tank.AddComponent<ProjectileEmitterComponent>(200, 500, 4000, false,
-- //   1, 0);
-- //   // Entity truck = registry->CreateEntity();
-- //   // truck.Group("enemy");
-- //   // truck.AddComponent<TransformComponent>(glm::vec2(750, 500),
-- //   //                                        glm::vec2(4.0, 4.0));
-- //   // truck.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
-- //   // truck.AddComponent<SpriteComponent>(32, 32, "truck", 2);
-- //   // truck.AddComponent<BoxColliderComponent>(glm::vec2(0, 0),
-- //   glm::vec2(32,
-- //   // 32)); truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(75,
-- //   75),
-- //   // 1000, 4000,
-- //   //                                                DOWN, false, 1);
-- //   // truck.AddComponent<HealthComponent>(20);

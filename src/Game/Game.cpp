#include "Game.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TextLabelComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CameraFollowSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Systems/ProjectileKillSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/RenderTextLabelSystem.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <fstream>
#include <glm/glm.hpp>

int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;

Game::Game() {
  isRunning = false;
  isDebug = false;
  registry = std::make_unique<Registry>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  Logger::Log("Game constructor");
}

Game::~Game() { Logger::Log("Game destructor"); }

void Game::Init() {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    Logger::Err("ERROR INITIALIZING SDL");
    return;
  }

  if (TTF_Init() == -1) {
    Logger::Err("ERROR INITIALIZING SDL TTF");
    return;
  }

  // Create window
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

  window = SDL_CreateWindow("Grapevine", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                            SDL_WINDOW_BORDERLESS);
  if (!window) {
    Logger::Err("ERROR CREATING SDL WINDOW");
    return;
  }
  // Create renderer
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    Logger::Err("ERROR CREATING SDL RENDERER");
    return;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  camera = {0, 0, windowWidth, windowHeight};

  isRunning = true;
}

void Game::LoadLevel(int levelNumber) {
  // Load level
  // Add systems to registry
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();
  registry->AddSystem<RenderTextLabelSystem>();
  registry->AddSystem<AnimationSystem>();
  registry->AddSystem<CollisionSystem>();
  registry->AddSystem<RenderCollisionSystem>();
  registry->AddSystem<DamageSystem>();
  registry->AddSystem<KeyboardMovementSystem>();
  registry->AddSystem<CameraFollowSystem>();
  registry->AddSystem<ProjectileEmitSystem>();
  registry->AddSystem<ProjectileKillSystem>();

  // Add assets to asset store
  assetStore->AddTexture(renderer, "tilemap", "assets/tilemaps/jungle.png");
  assetStore->AddTexture(renderer, "tank",
                         "assets/images/tank-panther-right.png");
  assetStore->AddTexture(renderer, "truck",
                         "assets/images/truck-ford-right.png");
  assetStore->AddTexture(renderer, "chopper",
                         "assets/images/chopper-spritesheet.png");
  assetStore->AddTexture(renderer, "radar", "assets/images/radar.png");
  assetStore->AddTexture(renderer, "projectile", "assets/images/bullet.png");

  // Load font to asset store
  assetStore->AddFont("chariot", "./assets/fonts/chariot.ttf", 48);
  assetStore->AddFont("Arial", "./assets/fonts/arial.ttf", 32);

  // Load the tilemap
  // Load tilemap png
  // Load .map file
  size_t tileSize = 32;
  double tileScale = 3.0;
  int mapColumns = 25;
  int mapRows = 20;
  std::fstream mapFile;
  mapFile.open("assets/tilemaps/jungle.map");

  if (!mapFile.is_open()) {
    Logger::Err("ERROR LOADING TILEMAP");
    return;
  }

  for (int row = 0; row < mapRows; row++) {
    for (int column = 0; column < mapColumns; column++) {
      char ch;
      mapFile.get(ch);
      int sourceRectY = atoi(&ch) * tileSize;
      mapFile.get(ch);
      int sourceRectX = atoi(&ch) * tileSize;
      mapFile.ignore();

      Entity tile = registry->CreateEntity();
      tile.Group("tile");
      tile.AddComponent<TransformComponent>(
          glm::vec2(column * tileSize * tileScale, row * tileSize * tileScale),
          glm::vec2(tileScale, tileScale));
      tile.AddComponent<SpriteComponent>(tileSize, tileSize, "tilemap", 0,
                                         false, sourceRectX, sourceRectY);
    }
  }
  mapFile.close();

  mapWidth = mapColumns * tileSize * tileScale;
  mapHeight = mapRows * tileSize * tileScale;

  Entity player = registry->CreateEntity();
  player.Tag("player");
  player.AddComponent<TransformComponent>(glm::vec2(20, 200),
                                          glm::vec2(2.0, 2.0));
  player.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  player.AddComponent<SpriteComponent>(32, 32, "chopper", 3, 0, 32);
  player.AddComponent<AnimationComponent>(2, 15, true);
  player.AddComponent<KeyboardControlComponent>(
      glm::vec2(0, -200), glm::vec2(200, 0), glm::vec2(0, 200),
      glm::vec2(-200, 0));
  player.AddComponent<CameraFollowComponent>();
  player.AddComponent<HealthComponent>(3);
  player.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32, 32));
  player.AddComponent<ProjectileEmitterComponent>(glm::vec2(400, 400), 0, 1500,
                                                  true, 1);

  Entity radar = registry->CreateEntity();
  radar.AddComponent<TransformComponent>(glm::vec2(1800.0, 10.0),
                                         glm::vec2(1.5, 1.5));
  radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  radar.AddComponent<SpriteComponent>(64, 64, "radar", 100, true);
  radar.AddComponent<AnimationComponent>(8, 10, true);

  Entity tank = registry->CreateEntity();
  tank.Group("enemy");
  tank.AddComponent<HealthComponent>(1);
  tank.AddComponent<TransformComponent>(glm::vec2(200, 20),
                                        glm::vec2(2.0, 2.0));
  tank.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0));
  tank.AddComponent<SpriteComponent>(32, 32, "tank", 3);
  tank.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32, 32));
  tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(0, 75), 500, 4000,
                                                false, 1);

  // Entity truck = registry->CreateEntity();
  // truck.AddComponent<TransformComponent>(glm::vec2(1000, 20),
  //                                        glm::vec2(4.0, 4.0));
  // truck.AddComponent<RigidBodyComponent>(glm::vec2(-50.0, 0.0));
  // truck.AddComponent<SpriteComponent>(32, 32, "truck", 2);
  // truck.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32,
  // 32));

  Entity label = registry->CreateEntity();
  label.Tag("label");
  label.AddComponent<TransformComponent>(glm::vec2(windowWidth / 2 - 200, 10),
                                         glm::vec2(1.0, 1.0));
  label.AddComponent<TextLabelComponent>("CHOPPPAAA", SDL_Color{255, 0, 0},
                                         "chariot", true);

  Entity HPLabel = registry->CreateEntity();
  HPLabel.Tag("HPLabel");
  HPLabel.AddComponent<TransformComponent>(glm::vec2(10, 10),
                                           glm::vec2(1.0, 1.0));
  std::string hp = "HP: " + std::to_string(3);
  HPLabel.AddComponent<TextLabelComponent>("HP: 3", SDL_Color{255, 255, 255},
                                           "Arial", true);
}

void Game::Setup() { LoadLevel(0); }

void Game::Run() {
  Setup();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    switch (sdlEvent.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
        isRunning = false;
      if (sdlEvent.key.keysym.sym == SDLK_d)
        isDebug = !isDebug;
      eventBus->EmitEvent<KeyPressEvent>(sdlEvent.key.keysym.sym);
      break;
    }
  }
}

void Game::Update() {
  int timeToWait = MILLIS_PER_FRAME - (SDL_GetTicks() - milisecondsPrevFrame);
  if (timeToWait > 0 && timeToWait <= MILLIS_PER_FRAME)
    SDL_Delay(timeToWait);

  double deltaTime = (SDL_GetTicks() - milisecondsPrevFrame) / 1000.0;

  milisecondsPrevFrame = SDL_GetTicks();

  eventBus->Reset();

  registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
  registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(eventBus);
  registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

  registry->Update();

  // invoke system update
  registry->GetSystem<MovementSystem>().Update(deltaTime);
  registry->GetSystem<AnimationSystem>().Update();
  registry->GetSystem<CollisionSystem>().Update(eventBus);
  registry->GetSystem<CameraFollowSystem>().Update(camera);
  registry->GetSystem<ProjectileEmitSystem>().Update(registry);
  registry->GetSystem<ProjectileKillSystem>().Update();
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // invoke system render
  registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
  registry->GetSystem<RenderTextLabelSystem>().Update(renderer, assetStore,
                                                      camera);
  if (isDebug)
    registry->GetSystem<RenderCollisionSystem>().Update(renderer, camera);

  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

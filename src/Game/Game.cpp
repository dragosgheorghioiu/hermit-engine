#include "Game.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/RenderSystem.h"
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

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  isRunning = true;
}

void Game::LoadLevel(int levelNumber) {
  // Load level
  // Add systems to registry
  registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();
  registry->AddSystem<AnimationSystem>();
  registry->AddSystem<CollisionSystem>();
  registry->AddSystem<RenderCollisionSystem>();
  registry->AddSystem<DamageSystem>();
  registry->AddSystem<KeyboardMovementSystem>();

  // Add assets to asset store
  assetStore->AddTexture(renderer, "tilemap", "assets/tilemaps/jungle.png");
  assetStore->AddTexture(renderer, "tank",
                         "assets/images/tank-panther-right.png");
  assetStore->AddTexture(renderer, "truck",
                         "assets/images/truck-ford-right.png");
  assetStore->AddTexture(renderer, "chopper",
                         "assets/images/chopper-spritesheet.png");
  assetStore->AddTexture(renderer, "radar", "assets/images/radar.png");

  // Load the tilemap
  // Load tilemap png
  // Load .map file
  size_t tileSize = 32;
  double tileScale = 2.0;
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
      tile.AddComponent<TransformComponent>(
          glm::vec2(column * tileSize * tileScale, row * tileSize * tileScale),
          glm::vec2(tileScale, tileScale));
      tile.AddComponent<SpriteComponent>(tileSize, tileSize, "tilemap", 0,
                                         sourceRectX, sourceRectY);
    }
  }
  mapFile.close();

  Entity player = registry->CreateEntity();
  player.AddComponent<TransformComponent>(glm::vec2(20, 1000),
                                          glm::vec2(1.5, 1.5));
  player.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
  int animationIndex = 1;
  player.AddComponent<SpriteComponent>(32, 32, "chopper", 3, 0,
                                       32 * animationIndex);
  player.AddComponent<AnimationComponent>(2, 10, true);

  Entity radar = registry->CreateEntity();
  radar.AddComponent<TransformComponent>(glm::vec2(1490.0, 10.0),
                                         glm::vec2(1.5, 1.5));
  radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
  radar.AddComponent<SpriteComponent>(64, 64, "radar", 100);
  radar.AddComponent<AnimationComponent>(8, 10, true);

  Entity tank = registry->CreateEntity();
  tank.AddComponent<TransformComponent>(glm::vec2(20, 20), glm::vec2(2.0, 2.0));
  tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0));
  tank.AddComponent<SpriteComponent>(32, 32, "tank", 3);
  tank.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32, 32));

  Entity truck = registry->CreateEntity();
  truck.AddComponent<TransformComponent>(glm::vec2(500, 20),
                                         glm::vec2(4.0, 4.0));
  truck.AddComponent<RigidBodyComponent>(glm::vec2(-50.0, 0.0));
  truck.AddComponent<SpriteComponent>(32, 32, "truck", 2);
  truck.AddComponent<BoxColliderComponent>(glm::vec2(0, 0), glm::vec2(32, 32));
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

  registry->Update();

  // invoke system update
  registry->GetSystem<MovementSystem>().Update(deltaTime);
  registry->GetSystem<AnimationSystem>().Update();
  registry->GetSystem<CollisionSystem>().Update(eventBus);
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // invoke system render
  registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
  if (isDebug)
    registry->GetSystem<RenderCollisionSystem>().Update(renderer);

  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

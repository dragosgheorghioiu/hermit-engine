#include "Game.h"
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
#include "../Systems/RenderGUISystem.h"
#include "../Systems/RenderHealthSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/RenderTextLabelSystem.h"
#include "LevelLoader.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <string>

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

  // init imgui
  ImGui::CreateContext();
  // set color
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer2_Init(renderer);

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  camera = {0, 0, windowWidth, windowHeight};

  isRunning = true;

  // hide mouse cursor
  // SDL_ShowCursor(SDL_DISABLE);
}

void Game::Setup() {
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
  registry->AddSystem<RenderHealthSystem>();
  registry->AddSystem<RenderGUISystem>();

  LevelLoader loader;
  lua.open_libraries(sol::lib::base, sol::lib::math);
  loader.LoadLevel(lua, 1, registry, assetStore, renderer);
}

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
    // ImGui event handling
    ImGui_ImplSDL2_ProcessEvent(&sdlEvent);

    // SDL event handling
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
    case SDL_KEYUP:
      eventBus->EmitEvent<KeyReleaseEvent>(sdlEvent.key.keysym.sym);
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
  registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
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
  registry->GetSystem<RenderHealthSystem>().Update(renderer, assetStore,
                                                   camera);
  if (isDebug) {
    // show hitboxes
    registry->GetSystem<RenderCollisionSystem>().Update(renderer, camera);

    // show imgui
    registry->GetSystem<RenderGUISystem>().Update(registry, assetStore, camera);
  }

  SDL_RenderPresent(renderer);
}

void Game::Destroy() {
  ImGui_ImplSDL2_Shutdown();
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui::DestroyContext();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

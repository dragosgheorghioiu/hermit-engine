#include "Game.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../SceneLoader/SceneLoader.h"
// #include "../Systems/AnimationSystem.h"
// #include "../Systems/CameraFollowSystem.h"
// #include "../Systems/CollisionSystem.h"
// #include "../Systems/DamageSystem.h"
// #include "../Systems/KeyboardMovementSystem.h"
// #include "../Systems/MovementSystem.h"
// #include "../Systems/ProjectileEmitSystem.h"
// #include "../Systems/ProjectileKillSystem.h"
// #include "../Systems/RenderCollisionSystem.h"
// #include "../Systems/RenderGUISystem.h"
// #include "../Systems/RenderHealthSystem.h"
#include "../Systems/RenderSystem.h"
// #include "../Systems/RenderTextLabelSystem.h"
// #include "../Systems/ScriptSystem.h"
#include "imgui_impl_sdlrenderer2.h"
#include "toml/parser.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
#include <glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <memory>
#include <string>

std::unique_ptr<PluginLoader> Game::pluginLoader;
toml::basic_value<toml::discard_comments, std::unordered_map, std::vector>
    Game::config_file;
std::filesystem::path Game::config_dir;
int Game::windowWidth;
int Game::windowHeight;
int Game::mapWidth;
int Game::mapHeight;
sol::state Game::lua;

Game::Game() {
  GetConfig();

  isRunning = false;
  isDebug = false;
  registry = std::make_unique<Registry>();
  pluginRegistry = std::make_unique<RegistryType>();
  assetStore = std::make_unique<AssetStore>();
  eventBus = std::make_unique<EventBus>();
  pluginLoader = std::make_unique<PluginLoader>();
  sceneLoader = std::make_unique<SceneLoader>();

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
  // TODO: BACK TO FULLSCREEN
  windowWidth = displayMode.w / 2;
  windowHeight = displayMode.h / 2;

  window = SDL_CreateWindow("Geck Engine", SDL_WINDOWPOS_CENTERED,
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

  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  camera = {0, 0, windowWidth, windowHeight};

  isRunning = true;

  // hide mouse cursor
  // SDL_ShowCursor(SDL_DISABLE);

  // Load plugins
  pluginLoader->loadEvents("../src/Plugin/Events/PluginsToLoad/");
  pluginLoader->loadComponents("../src/Plugin/Components/PluginsToLoad/");
  pluginLoader->loadSystems("../src/Plugin/Systems/PluginsToLoad/",
                            pluginRegistry.get());

  pluginLoader->getEventFactory().subscribe(
      "PluginEvent", &pluginRegistry->getPluginSystem("DemoPlugin2"));
  pluginLoader->getEventFactory().subscribe(
      "PluginEvent", &pluginRegistry->getPluginSystem("DemoPlugin"));
  pluginLoader->getEventFactory().triggerEvent("PluginEvent", 10);
}

void Game::setComponentSignatureOfSystem(std::string systemName) {
  const char **requiredComponents =
      pluginRegistry->getPluginSystem(systemName).requiredComponents;
  while (*requiredComponents) {
    std::string str = *requiredComponents++;
    int id = pluginLoader->getComponentInfo(str).getId();
    pluginRegistry->getPluginSystem(systemName)
        .instance->changeComponentSignature(id);
  }
  Logger::Log("Component signature set for system: " + systemName);
}

void Game::Setup() {
  setComponentSignatureOfSystem("DemoPlugin2");
  setComponentSignatureOfSystem("DemoPlugin");
  setComponentSignatureOfSystem("PluginRenderSystem");
  // Add systems to registry
  // registry->AddSystem<MovementSystem>();
  registry->AddSystem<RenderSystem>();
  // registry->AddSystem<RenderTextLabelSystem>();
  // registry->AddSystem<AnimationSystem>();
  // registry->AddSystem<CollisionSystem>();
  // registry->AddSystem<RenderCollisionSystem>();
  // registry->AddSystem<DamageSystem>();
  // registry->AddSystem<KeyboardMovementSystem>();
  // registry->AddSystem<CameraFollowSystem>();
  // registry->AddSystem<ProjectileEmitSystem>();
  // registry->AddSystem<ProjectileKillSystem>();
  // registry->AddSystem<RenderHealthSystem>();
  // registry->AddSystem<RenderGUISystem>();
  // registry->AddSystem<ScriptSystem>();

  // create lua bindings
  // registry->GetSystem<ScriptSystem>().CreateLuaBindings(registry, assetStore,
  //                                                       lua);

  ComponentFactoryInfo pluginComponent =
      pluginLoader->getComponentInfo("PluginComponent");

  EntityType entity = pluginRegistry->createEntity();
  // entity.addComponent(pluginComponent, 1);

  EntityType entity2 = pluginRegistry->createEntity();
  entity2.addComponent(pluginComponent, 2);

  EntityType entity3 = pluginRegistry->createEntity();
  // entity3.addComponent(pluginComponent, 99);

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
  sceneLoader->LoadScene("sceneA.toml", registry, pluginRegistry, assetStore,
                         renderer);
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
        pluginLoader->getEventFactory().triggerEvent("PluginEvent", 10);
      // isDebug = !isDebug;
      // eventBus->EmitEvent<KeyPressEvent>(sdlEvent.key.keysym.sym);
      break;
    case SDL_KEYUP:
      // eventBus->EmitEvent<KeyReleaseEvent>(sdlEvent.key.keysym.sym);
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

  // registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
  // registry->GetSystem<MovementSystem>().SubscribeToEvents(eventBus);
  // registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(eventBus);
  // registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventBus);

  registry->Update();
  pluginRegistry->update();

  // invoke system update
  // registry->GetSystem<MovementSystem>().Update(deltaTime);
  // registry->GetSystem<AnimationSystem>().Update();
  // registry->GetSystem<CollisionSystem>().Update(eventBus);
  // registry->GetSystem<CameraFollowSystem>().Update(camera);
  // registry->GetSystem<ProjectileEmitSystem>().Update(registry);
  // registry->GetSystem<ProjectileKillSystem>().Update();
  // registry->GetSystem<ScriptSystem>().Update(deltaTime,
  // milisecondsPrevFrame);

  // run plugin update
  // pluginRegistry->callPluginSystemUpdate("DemoPlugin2", {&counter});
  // Logger::Log("Counter: " + std::to_string(counter));
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // invoke system render
  // registry->GetSystem<RenderSystem>().Update(renderer, assetStore, camera);
  // registry->GetSystem<RenderTextLabelSystem>().Update(renderer, assetStore,
  // camera);
  // registry->GetSystem<RenderHealthSystem>().Update(renderer, assetStore,
  //                                                  camera);
  if (isDebug) {
    // show hitboxes
    // registry->GetSystem<RenderCollisionSystem>().Update(renderer, camera);

    // show imgui
    // registry->GetSystem<RenderGUISystem>().Update(registry, assetStore,
    // camera, lua);
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

void Game::GetConfig() {
  config_dir = std::filesystem::current_path().parent_path().append("config");

  try {
    config_file = toml::parse(config_dir.append("config.toml"));
  } catch (toml::syntax_error err) {
    Logger::Err("Could not parse config file");
    return;
  }
}

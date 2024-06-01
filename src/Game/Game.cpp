#include "Game.h"
#include "../Logger/Logger.h"
#include "../SceneLoader/SceneLoader.h"
#include "imgui_impl_sdlrenderer2.h"
#include "toml/parser.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <filesystem>
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
  pluginRegistry = std::make_unique<RegistryType>();
  assetStore = std::make_unique<AssetStore>();
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
  windowWidth = displayMode.w;
  windowHeight = displayMode.h;

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

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

  camera = {0, 0, windowWidth, windowHeight};

  isRunning = true;

  // hide mouse cursor
  // SDL_ShowCursor(SDL_DISABLE);

  // Load plugins
  pluginLoader->loadEvents("../src/Plugin/Events/PluginsToLoad/");
  pluginLoader->loadComponents("../src/Plugin/Components/PluginsToLoad/");
  pluginLoader->loadSystems("../src/Plugin/Systems/PluginsToLoad/",
                            pluginRegistry.get(), &lua);
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

void Game::addGUIElement(std::string systemName) {
  std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
      guiElements = pluginRegistry->getPluginSystem(systemName)
                        .instance->getGUIElements();
  for (auto const &[key, value] : guiElements) {
    allGuiElements[key] = value;
  }
}

void Game::Setup() {
  setComponentSignatureOfSystem("PluginRenderSystem");
  setComponentSignatureOfSystem("PluginAnimationSystem");
  setComponentSignatureOfSystem("PluginMovementSystem");
  setComponentSignatureOfSystem("RenderCollisionSystem");
  setComponentSignatureOfSystem("CollisionSystem");
  setComponentSignatureOfSystem("KeyboardControlSystem");

  pluginLoader->getEventFactory().subscribe(
      "collisionEvent",
      &pluginRegistry->getPluginSystem("PluginMovementSystem"));
  pluginLoader->getEventFactory().subscribe(
      "keyPressEvent",
      &pluginRegistry->getPluginSystem("KeyboardControlSystem"));
  pluginLoader->getEventFactory().subscribe(
      "keyReleaseEvent",
      &pluginRegistry->getPluginSystem("KeyboardControlSystem"));

  addGUIElement("PluginAnimationSystem");

  ComponentFactoryInfo pluginComponent =
      pluginLoader->getComponentInfo("PluginComponent");
  ComponentFactoryInfo transformComponent =
      pluginLoader->getComponentInfo("TransformComponent");
  ComponentFactoryInfo boxColliderComponent =
      pluginLoader->getComponentInfo("BoxColliderComponent");

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os);
  sceneLoader->LoadScene("sceneA.toml", pluginRegistry, pluginLoader,
                         assetStore, renderer);
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
      if (sdlEvent.key.keysym.sym == SDLK_d) {
        pluginLoader->getEventFactory().triggerEvent("PluginEvent", 10);
        isDebug = !isDebug;
      }
      pluginLoader->getEventFactory().triggerEvent("keyPressEvent",
                                                   sdlEvent.key.keysym.sym);
      break;
    case SDL_KEYUP:
      pluginLoader->getEventFactory().triggerEvent("keyReleaseEvent",
                                                   sdlEvent.key.keysym.sym);
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

  pluginRegistry->update();

  // invoke system update
  pluginRegistry->callPluginSystemUpdate("PluginAnimationSystem", {});
  pluginRegistry->callPluginSystemUpdate("PluginMovementSystem", {&deltaTime});
  pluginRegistry->callPluginSystemUpdate("CollisionSystem",
                                         {pluginLoader.get()});
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // invoke system render
  pluginRegistry->callPluginSystemUpdate("PluginRenderSystem",
                                         {renderer, &assetStore, &camera});
  pluginRegistry->callPluginSystemUpdate("RenderCollisionSystem",
                                         {renderer, &camera});
  if (isDebug) {
    // setup imgui render window
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    // render imgui window
    showMouseCursorPositionPanel();

    // render imgui elements from plugin systems
    ImGuiContext *ctx = ImGui::GetCurrentContext();
    for (auto const &[key, value] : allGuiElements) {
      value(ctx);
    }

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
  }
  SDL_RenderPresent(renderer);
}

void Game::showMouseCursorPositionPanel() {
  // show mouse position panel
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(300, 300));
  ImGuiModFlags flags = ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoDecoration;
  if (ImGui::Begin("Mouse Position", nullptr, flags)) {
    ImGui::Text("Mouse Position: (%.1f,%.1f)",
                ImGui::GetIO().MousePos.x + camera.x,
                ImGui::GetIO().MousePos.y + camera.y);
  }
  ImGui::End();
}
void Game::Destroy() {
  allGuiElements.clear();
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

#include "Game.h"
#include "../Logger/Logger.h"
#include "../SceneLoader/SceneLoader.h"
#include "imgui_impl_sdlrenderer2.h"
#include "toml/parser.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_keycode.h>
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
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
  pluginLoader->loadComponents("../src/Plugin/Components/PluginsToLoad/", lua);
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

  lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::os,
                     sol::lib::package, sol::lib::string, sol::lib::table);
  setLuaMappings();
  sceneLoader->LoadScene("sceneA.toml", pluginRegistry, pluginLoader,
                         assetStore, renderer);
  lua["setup"](pluginRegistry.get());
}

void Game::Run() {
  Setup();
  Render();
  while (isRunning) {
    ProcessInput();
    Update();
    Render();
  }
}

void Game::ProcessInput() {
  SDL_Event sdlEvent;
  while (SDL_PollEvent(&sdlEvent)) {
    // stop other input processing if in debug mode
    if (isDebug) {
      // ImGui event handling
      ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    }

    // SDL event handling
    switch (sdlEvent.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    case SDL_KEYDOWN:
      if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
        isRunning = false;
      if (sdlEvent.key.keysym.sym == SDLK_d) {
        isDebug = !isDebug;
      }
      if (isDebug) {
        return;
      }
      pluginLoader->getEventFactory().triggerEvent("keyPressEvent",
                                                   {sdlEvent.key.keysym.sym});
      break;
    case SDL_KEYUP:
      pluginLoader->getEventFactory().triggerEvent("keyReleaseEvent",
                                                   {sdlEvent.key.keysym.sym});
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

  if (isDebug) {
    return;
  }

  lua["update"](*pluginRegistry.get(), deltaTime);
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
    showPropertyEditor();
    ImGui::ShowDemoWindow();

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

void Game::showPropertyEditor() {
  if (ImGui::Begin("Entity Property editor")) {
    if (ImGui::BeginTabBar("EntitiesTabBar")) {

      if (ImGui::BeginTabItem("Tags")) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();
        ImGui::Text("Entity Tag");
        ImGui::NextColumn();
        ImGui::Text("Properties");
        ImGui::NextColumn();
        ImGui::Separator();

        for (const auto &tag : pluginRegistry->getAllTags()) {
          if (tag.empty()) {
            continue;
          }
          ImGui::Text("%s", tag.c_str());
        }

        ImGui::Columns(1);
        ImGui::PopStyleVar();
        ImGui::EndTabItem();
      }

      if (ImGui::BeginTabItem("Groups")) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();
        ImGui::Text("Group");
        ImGui::NextColumn();
        ImGui::Text("Properties");
        ImGui::NextColumn();
        ImGui::Separator();

        for (const auto &group : pluginRegistry->getAllGroups()) {
          ImGui::Text("%s", group.c_str());
        }

        ImGui::Columns(1);
        ImGui::PopStyleVar();
        ImGui::EndTabItem();
      }

      ImGui::EndTabBar();
    }

    ImGui::Columns(1);
    ImGui::Separator();

    // put a button in the center of the window
    ImGui::SetCursorPosX(
        (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Create Entity").x) /
        2);
    if (ImGui::Button("Reload Scene")) {
      try {
        pluginRegistry->clear();
        Logger::Log("Reloading scene");
        sceneLoader->LoadScene("sceneA.toml", pluginRegistry, pluginLoader,
                               assetStore, renderer);
        lua["setup"](pluginRegistry.get());
      } catch (std::exception &e) {
        Logger::Err(e.what());
      }
    }
  }

  ImGui::End();
}

void Game::showMouseCursorPositionPanel() {
  // show mouse position panel
  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
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

void Game::setLuaMappings() {
  // load main script
  // get current path
  std::filesystem::path currentPath = std::filesystem::current_path();
  // modify package.path to include the scripts directory
  lua["package"]["path"] =
      lua["package"]["path"].get<std::string>() + ";" +
      currentPath.parent_path().append("scripts").string() + "/?.lua";
  // get the path to the main.lua script
  std::filesystem::path mainScriptPath =
      currentPath.parent_path().append("scripts").append("main.lua");
  lua.script_file(mainScriptPath.string());

  // create lua user types for core engine classes
  EntityType::createLuaUserType(lua);
  ComponentInfo::createLuaUserType(lua);
  RegistryType::createLuaUserType(lua);

  // additional lua functions
  lua.set_function(
      "trigger_event", [&](std::string eventName, sol::variadic_args args) {
        std::vector<std::any> params;
        for (auto arg : args) {
          params.push_back(solObjectToStdAny(arg));
        }
        pluginLoader->getEventFactory().triggerEvent(eventName, params);
      });
  lua.set_function("get_entity_by_tag", [&](std::string tag) {
    return pluginRegistry->getEntityByTag(tag);
  });
  lua.set_function("get_config", [&]() { return config_file; });
  createLuaTableForKeys();
}

std::any Game::solObjectToStdAny(const sol::object &obj) {
  switch (obj.get_type()) {
  case sol::type::boolean:
    return obj.as<bool>();
  case sol::type::number:
    if (obj.is<int>()) {
      return obj.as<int>();
    }
    if (obj.is<float>()) {
      return obj.as<float>();
    }
    if (obj.is<double>()) {
      return obj.as<double>();
    }
  case sol::type::string:
    return obj.as<const char *>();
  case sol::type::table:
    return obj.as<sol::table>();
  case sol::type::function:
    return obj.as<sol::function>();
  case sol::type::userdata:
    return obj.as<sol::userdata>();
  case sol::type::lightuserdata:
    return obj.as<void *>();
  case sol::type::thread:
    return obj.as<sol::thread>();
  case sol::type::nil:
  default:
    return std::any();
  }
}

void Game::createLuaTableForKeys() {
  // create enum for sdl keys in lua
  lua.new_enum(
      "keyboard_input", "Q", SDLK_q, "W", SDLK_w, "E", SDLK_e, "R", SDLK_r, "T",
      SDLK_t, "Y", SDLK_y, "U", SDLK_u, "I", SDLK_i, "O", SDLK_o, "P", SDLK_p,
      "A", SDLK_a, "S", SDLK_s, "D", SDLK_d, "F", SDLK_f, "G", SDLK_g, "H",
      SDLK_h, "J", SDLK_j, "K", SDLK_k, "L", SDLK_l, "Z", SDLK_z, "X", SDLK_x,
      "C", SDLK_c, "V", SDLK_v, "B", SDLK_b, "N", SDLK_n, "M", SDLK_m, "1",
      SDLK_1, "2", SDLK_2, "3", SDLK_3, "4", SDLK_4, "5", SDLK_5, "6", SDLK_6,
      "7", SDLK_7, "8", SDLK_8, "9", SDLK_9, "0", SDLK_0, "RETURN", SDLK_RETURN,
      "ESCAPE", SDLK_ESCAPE, "BACKSPACE", SDLK_BACKSPACE, "TAB", SDLK_TAB,
      "SPACE", SDLK_SPACE, "MINUS", SDLK_MINUS, "EQUALS", SDLK_EQUALS,
      "LEFTBRACKET", SDLK_LEFTBRACKET, "RIGHTBRACKET", SDLK_RIGHTBRACKET,
      "BACKSLASH", SDLK_BACKSLASH, "SEMICOLON", SDLK_SEMICOLON, "COMMA",
      SDLK_COMMA, "PERIOD", SDLK_PERIOD, "SLASH", SDLK_SLASH, "CAPSLOCK",
      SDLK_CAPSLOCK, "LEFT", SDLK_LEFT, "RIGHT", SDLK_RIGHT, "UP", SDLK_UP,
      "DOWN", SDLK_DOWN, "SHIFT", SDLK_LSHIFT, "CTRL", SDLK_LCTRL, "ALT",
      SDLK_LALT, "F1", SDLK_F1, "F2", SDLK_F2, "F3", SDLK_F3, "F4", SDLK_F4,
      "F5", SDLK_F5, "F6", SDLK_F6, "F7", SDLK_F7, "F8", SDLK_F8, "F9", SDLK_F9,
      "F10", SDLK_F10, "F11", SDLK_F11, "F12", SDLK_F12, "PRINTSCREEN",
      SDLK_PRINTSCREEN, "SCROLLLOCK", SDLK_SCROLLLOCK, "PAUSE", SDLK_PAUSE,
      "INSERT", SDLK_INSERT, "HOME", SDLK_HOME, "PAGEUP", SDLK_PAGEUP, "DELETE",
      SDLK_DELETE, "END", SDLK_END, "PAGEDOWN", SDLK_PAGEDOWN, "NUMLOCKCLEAR",
      SDLK_NUMLOCKCLEAR, "KP_DIVIDE", SDLK_KP_DIVIDE, "KP_MULTIPLY",
      SDLK_KP_MULTIPLY, "KP_MINUS", SDLK_KP_MINUS, "KP_PLUS", SDLK_KP_PLUS,
      "KP_ENTER", SDLK_KP_ENTER, "KP_1", SDLK_KP_1);
}

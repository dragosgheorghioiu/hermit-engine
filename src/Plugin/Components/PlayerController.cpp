#include "PlayerController.h"
#include "../PluginComponentFactory.h"
#include <sol/state.hpp>
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params

) {
  bool ignoreInput = std::get<bool>(component_params[0]);

  PlayerController *playerController = new PlayerController(ignoreInput);

  return static_cast<void *>(playerController);
}

extern "C" void destroyInstance(void *instance) {
  delete static_cast<PlayerController *>(instance);
}

extern "C" const char *getComponentName() { return "PlayerController"; }

extern "C" void addLuaBindings(sol::state &lua) {
  // ignore input
  lua.set_function("get_player_controller_ignore_input",
                   [](ComponentInfo *component) {
                     auto playerController =
                         static_cast<PlayerController *>(component->instance);
                     return playerController->ignoreInput;
                   });
  lua.set_function(
      "set_player_controller_ignore_input", [](ComponentInfo *component) {
        auto playerController =
            static_cast<PlayerController *>(component->instance);
        playerController->ignoreInput = !playerController->ignoreInput;
      });
}

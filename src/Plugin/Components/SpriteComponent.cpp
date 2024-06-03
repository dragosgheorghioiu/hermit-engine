#include "SpriteComponent.h"
#include "../PluginComponentFactory.h"
#include <sol/state.hpp>
#include <variant>
#include <vector>

extern "C" void *createInstance(
    std::vector<std::variant<int, bool, float, const char *, std::vector<int>,
                             std::vector<bool>, std::vector<float>>>
        component_params) {
  int width = std::get<int>(component_params[0]);
  int height = std::get<int>(component_params[1]);
  const char *id = std::get<const char *>(component_params[2]);
  unsigned int zIndex = std::get<int>(component_params[3]);
  bool isFixed = std::get<bool>(component_params[4]);
  int srcRectX = std::get<int>(component_params[5]);
  int srcRectY = std::get<int>(component_params[6]);

  PluginSpriteComponent *sprite = new PluginSpriteComponent(
      width, height, id, zIndex, isFixed, srcRectX, srcRectY);
  return static_cast<void *>(sprite);
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginSpriteComponent *>(instance);
}
extern "C" const char *getComponentName() { return "SpriteComponent"; }

extern "C" void addLuaBindings(sol::state &lua) {
  // sprite width
  lua.set_function("get_sprite_width", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->width;
  });
  lua.set_function("set_sprite_width", [](ComponentInfo *component, int width) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    spriteComponent->width = width;
    spriteComponent->srcRect.w = width;
  });

  // sprite height
  lua.set_function("get_sprite_height", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->height;
  });
  lua.set_function(
      "set_sprite_height", [](ComponentInfo *component, int height) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->height = height;
        spriteComponent->srcRect.h = height;
      });

  // sprite zIndex
  lua.set_function("get_sprite_zIndex", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->zIndex;
  });
  lua.set_function(
      "set_sprite_zIndex", [](ComponentInfo *component, int zIndex) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->zIndex = zIndex;
      });

  // sprite id
  lua.set_function("get_sprite_id", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->id;
  });
  lua.set_function(
      "set_sprite_id", [](ComponentInfo *component, const char *id) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->id = id;
      });

  // sprite isFixed
  lua.set_function("get_sprite_isFixed", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->isFixed;
  });
  lua.set_function(
      "set_sprite_isFixed", [](ComponentInfo *component, bool isFixed) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->isFixed = isFixed;
      });

  // sprite srcRectX
  lua.set_function("get_sprite_srcRectX", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->srcRect.x;
  });
  lua.set_function(
      "set_sprite_srcRectX", [](ComponentInfo *component, int srcRectX) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->srcRect.x = srcRectX;
      });

  // sprite srcRectY
  lua.set_function("get_sprite_srcRectY", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->srcRect.y;
  });
  lua.set_function(
      "set_sprite_srcRectY", [](ComponentInfo *component, int srcRectY) {
        auto spriteComponent =
            static_cast<PluginSpriteComponent *>(component->instance);
        spriteComponent->srcRect.y = srcRectY;
      });

  // sprite flip
  lua.set_function("get_sprite_flip", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    return spriteComponent->flip == SDL_FLIP_HORIZONTAL;
  });
  lua.set_function("set_sprite_flip_h", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    spriteComponent->flip = SDL_FLIP_HORIZONTAL;
  });
  lua.set_function("set_sprite_flip_v", [](ComponentInfo *component) {
    auto spriteComponent =
        static_cast<PluginSpriteComponent *>(component->instance);
    spriteComponent->flip = SDL_FLIP_VERTICAL;
  });
}

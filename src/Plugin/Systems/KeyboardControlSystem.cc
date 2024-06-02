#include "KeyboardControlSystem.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Events/DemoEvent.h"
#include "../Events/KeyboardPressEvent.h"
#include "../Events/KeyboardReleaseEvent.h"
#include "glm/fwd.hpp"
#include <SDL2/SDL_keycode.h>

KeyboardControlSystem::KeyboardControlSystem() = default;
KeyboardControlSystem::~KeyboardControlSystem() = default;

void KeyboardControlSystem::update(std::vector<void *> params) {}

const std::string KeyboardControlSystem::GetName() const {
  return "KeyboardControlSystem";
}

std::function<void(void *)>
KeyboardControlSystem::getCallback(std::string eventType) {
  if (eventType == "keyPressEvent") {
    return [this](void *event) { this->onKeyPress(event); };
  } else if (eventType == "keyReleaseEvent") {
    return [this](void *event) { this->onKeyRelease(event); };
  } else if (eventType == "PluginEvent") {
    return [this](void *event) { this->onPluginEvent(event); };
  }
  return nullptr;
}

void KeyboardControlSystem::onPluginEvent(void *event) {
  auto pluginEvent = static_cast<DemoEvent *>(event);

  std::cout << "Plugin Event: " << pluginEvent->value << " "
            << pluginEvent->value2 << " " << pluginEvent->str << std::endl;
}

void KeyboardControlSystem::onKeyPress(void *event) {
  auto keyEvent = static_cast<KeyPressEvent *>(event);
  auto entities = getSystemEntities();

  for (auto &entity : entities) {
    if (entity.hasTag("player")) {
      auto key = keyEvent->keyCode;
      RigidBodyComponent *rigidBodyComponent =
          static_cast<RigidBodyComponent *>(
              entity.getComponent("RigidBodyComponent").instance);
      AnimationComponent *animationComponent =
          static_cast<AnimationComponent *>(
              entity.getComponent("AnimationComponent").instance);
      glm::vec2 velocity = rigidBodyComponent->velocity;
      glm::vec2 acceleration = rigidBodyComponent->acceleration;

      if (key == SDLK_RIGHT) {
        pressedKeys[0] = true;
      } else if (key == SDLK_LEFT) {
        pressedKeys[1] = true;
      } else if (key == SDLK_SPACE) {
      }
    }
  }
}

void KeyboardControlSystem::onKeyRelease(void *event) {
  auto keyEvent = static_cast<KeyReleaseEvent *>(event);
  auto key = keyEvent->keyCode;
  for (auto &entity : getSystemEntities()) {
    if (entity.hasTag("player")) {
      if (key == SDLK_RIGHT) {
        pressedKeys[0] = false;
      } else if (key == SDLK_LEFT) {
        pressedKeys[1] = false;
      } else if (key == SDLK_SPACE) {
      }
      if (!pressedKeys[0] && !pressedKeys[1]) {
        static_cast<RigidBodyComponent *>(
            entity.getComponent("RigidBodyComponent").instance)
            ->velocity.x = 0;
        static_cast<AnimationComponent *>(
            entity.getComponent("AnimationComponent").instance)
            ->animationIndex = 0;
      }
    }
  }
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
KeyboardControlSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
}

void KeyboardControlSystem::demoWindow() {}

extern "C" void *createInstance() { return new KeyboardControlSystem(); }

extern "C" void destroyInstance(void *instance) {
  delete (KeyboardControlSystem *)instance;
}

extern "C" const char *getName() { return "KeyboardControlSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"SpriteComponent", "TransformComponent",
                                     "RigidBodyComponent", nullptr};
  return components;
}

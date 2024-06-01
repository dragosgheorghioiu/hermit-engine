#include "KeyboardControlSystem.h"
#include "../Components/AnimationComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Events/KeyboardPressEvent.h"
#include "../Events/KeyboardReleaseEvent.h"
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
  }
  return nullptr;
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
      PluginSpriteComponent *spriteComponent =
          static_cast<PluginSpriteComponent *>(
              entity.getComponent("SpriteComponent").instance);
      AnimationComponent *animationComponent =
          static_cast<AnimationComponent *>(
              entity.getComponent("AnimationComponent").instance);
      if (key == SDLK_RIGHT) {
        rigidBodyComponent->velocity.x = 400;
        pressedKeys[0] = true;
        spriteComponent->flip = SDL_FLIP_NONE;
      } else if (key == SDLK_LEFT) {
        rigidBodyComponent->velocity.x = -400;
        spriteComponent->flip = SDL_FLIP_HORIZONTAL;
        pressedKeys[1] = true;
        animationComponent->animationIndex = 1;
      } else if (key == SDLK_SPACE) {
        if (rigidBodyComponent->isGrounded) {
          rigidBodyComponent->velocity.y = -350;
          rigidBodyComponent->isGrounded = false;
        }
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

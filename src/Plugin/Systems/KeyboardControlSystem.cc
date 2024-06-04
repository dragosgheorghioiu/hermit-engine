#include "KeyboardControlSystem.h"
#include "../Components/PlayerController.h"
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
  int key = keyEvent->keyCode;

  for (auto &entity : getSystemEntities()) {
    PlayerController *playerController = static_cast<PlayerController *>(
        entity.getComponent("PlayerController").instance);
    if (playerController->ignoreInput) {
      continue;
    }
    (*lua)["on_key_press"](entity, key);
  }
}

void KeyboardControlSystem::onKeyRelease(void *event) {
  auto keyEvent = static_cast<KeyReleaseEvent *>(event);
  auto key = keyEvent->keyCode;

  for (auto &entity : getSystemEntities()) {
    PlayerController *playerController = static_cast<PlayerController *>(
        entity.getComponent("PlayerController").instance);
    if (playerController->ignoreInput) {
      continue;
    }
    (*lua)["on_key_release"](entity, key);
  }
}

std::unordered_map<std::string, std::function<void(ImGuiContext *)>>
KeyboardControlSystem::getGUIElements() {
  return std::unordered_map<std::string, std::function<void(ImGuiContext *)>>();
}

extern "C" void *createInstance() { return new KeyboardControlSystem(); }

extern "C" void destroyInstance(void *instance) {
  delete (KeyboardControlSystem *)instance;
}

extern "C" const char *getName() { return "KeyboardControlSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"PlayerController", nullptr};
  return components;
}

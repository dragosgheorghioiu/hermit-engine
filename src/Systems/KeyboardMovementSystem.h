#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardPressEvent.h"
#include <SDL2/SDL_keyboard.h>

class KeyboardMovementSystem : public System {
public:
  KeyboardMovementSystem() { RequireComponent<TransformComponent>(); }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressEvent>(
        this, &KeyboardMovementSystem::onKeyboardInput);
  }

  void onKeyboardInput(KeyPressEvent &e) {
    Logger::Log("Key " + std::to_string(e.keyCode) + " with the symbol: " +
                SDL_GetKeyName(e.keyCode) + " was pressed");
  }
  void Update() {}
};

#endif

#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../Components/KeyboardControlComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardPressEvent.h"
#include "../Events/KeyboardReleaseEvent.h"
#include <SDL2/SDL_keyboard.h>

class KeyboardMovementSystem : public System {
public:
  KeyboardMovementSystem() {
    RequireComponent<KeyboardControlComponent>();
    RequireComponent<SpriteComponent>();
    RequireComponent<RigidBodyComponent>();
  }

  void SubscribeToEvents(std::unique_ptr<EventBus> &eventBus) {
    eventBus->SubscribeToEvent<KeyPressEvent>(
        this, &KeyboardMovementSystem::onKeyboardInput);
    eventBus->SubscribeToEvent<KeyReleaseEvent>(
        this, &KeyboardMovementSystem::onKeyboardRelease);
  }

  void onKeyboardInput(KeyPressEvent &e) {
    for (Entity entity : GetSystemEntities()) {
      const KeyboardControlComponent keyboardControl =
          entity.GetComponent<KeyboardControlComponent>();
      SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();
      RigidBodyComponent &rigidBody = entity.GetComponent<RigidBodyComponent>();

      // stop if not pressing any key
      if (e.keyCode == SDLK_UP || e.keyCode == SDLK_RIGHT ||
          e.keyCode == SDLK_DOWN || e.keyCode == SDLK_LEFT) {
        rigidBody.velocity = {0, 0};
      }
      switch (e.keyCode) {
      case SDLK_UP:
        rigidBody.velocity = keyboardControl.upVelocity;
        sprite.srcRect.y = sprite.height * 0;
        break;
      case SDLK_RIGHT:
        rigidBody.velocity = keyboardControl.rightVelocity;
        sprite.srcRect.y = sprite.height * 1;
        break;
      case SDLK_DOWN:
        rigidBody.velocity = keyboardControl.downVelocity;
        sprite.srcRect.y = sprite.height * 2;
        break;
      case SDLK_LEFT:
        rigidBody.velocity = keyboardControl.leftVelocity;
        sprite.srcRect.y = sprite.height * 3;
        break;
      }
    }
  }

  void onKeyboardRelease(KeyReleaseEvent &e) {
    for (Entity entity : GetSystemEntities()) {
      RigidBodyComponent &rigidBody = entity.GetComponent<RigidBodyComponent>();

      rigidBody.velocity = {0, 0};
    }
  }

  void Update() {}
};

#endif

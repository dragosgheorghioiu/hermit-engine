#ifndef KEYBOARD_MOVEMENT_SYSTEM_H
#define KEYBOARD_MOVEMENT_SYSTEM_H

#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
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

      switch (e.keyCode) {
      case SDLK_UP:
        rigidBody.velocity = keyboardControl.upVelocity;
        sprite.srcRect.y = sprite.height * 0;
        updateDirection(entity, UP);
        break;
      case SDLK_RIGHT:
        rigidBody.velocity = keyboardControl.rightVelocity;
        sprite.srcRect.y = sprite.height * 1;
        updateDirection(entity, RIGHT);
        break;
      case SDLK_DOWN:
        rigidBody.velocity = keyboardControl.downVelocity;
        sprite.srcRect.y = sprite.height * 2;
        updateDirection(entity, DOWN);
        break;
      case SDLK_LEFT:
        rigidBody.velocity = keyboardControl.leftVelocity;
        sprite.srcRect.y = sprite.height * 3;
        updateDirection(entity, LEFT);
        break;
      }
    }
  }

  void updateDirection(Entity &entity, Direction direction) {
    if (entity.HasComponent<ProjectileEmitterComponent>()) {
      auto &emitter = entity.GetComponent<ProjectileEmitterComponent>();
      emitter.direction = direction;
    }
  }
  void onKeyboardRelease(KeyReleaseEvent &e) {
    for (Entity entity : GetSystemEntities()) {
      RigidBodyComponent &rigidBody = entity.GetComponent<RigidBodyComponent>();

      switch (e.keyCode) {
      case SDLK_UP:
        if (rigidBody.velocity.y < 0) {
          rigidBody.velocity.y = 0;
        }
        break;
      case SDLK_RIGHT:
        if (rigidBody.velocity.x > 0) {
          rigidBody.velocity.x = 0;
        }
        break;
      case SDLK_DOWN:
        if (rigidBody.velocity.y > 0) {
          rigidBody.velocity.y = 0;
        }
        break;
      case SDLK_LEFT:
        if (rigidBody.velocity.x < 0) {
          rigidBody.velocity.x = 0;
        }
        break;
      }
    }
  }

  void Update() {}
};

#endif

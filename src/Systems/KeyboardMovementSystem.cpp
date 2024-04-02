#include "KeyboardMovementSystem.h"
#include "../Components/KeyboardControlComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

KeyboardMovementSystem::KeyboardMovementSystem() {
  RequireComponent<KeyboardControlComponent>();
  RequireComponent<SpriteComponent>();
  RequireComponent<RigidBodyComponent>();
}

void KeyboardMovementSystem::SubscribeToEvents(
    std::unique_ptr<EventBus> &eventBus) {
  eventBus->SubscribeToEvent<KeyPressEvent>(
      this, &KeyboardMovementSystem::onKeyboardInput);
  eventBus->SubscribeToEvent<KeyReleaseEvent>(
      this, &KeyboardMovementSystem::onKeyboardRelease);
}

void KeyboardMovementSystem::updateAngle(Entity &entity, const double &angle) {
  if (entity.HasComponent<ProjectileEmitterComponent>()) {
    auto &emitter = entity.GetComponent<ProjectileEmitterComponent>();
    emitter.angle = angle;
  }
}

void KeyboardMovementSystem::onKeyboardInput(KeyPressEvent &e) {
  for (Entity entity : GetSystemEntities()) {
    const KeyboardControlComponent keyboardControl =
        entity.GetComponent<KeyboardControlComponent>();
    SpriteComponent &sprite = entity.GetComponent<SpriteComponent>();
    RigidBodyComponent &rigidBody = entity.GetComponent<RigidBodyComponent>();

    switch (e.keyCode) {
    case SDLK_UP:
      rigidBody.velocity = keyboardControl.upVelocity;
      sprite.srcRect.y = sprite.height * 0;
      updateAngle(entity, M_PI / 2);
      break;
    case SDLK_RIGHT:
      rigidBody.velocity = keyboardControl.rightVelocity;
      sprite.srcRect.y = sprite.height * 1;
      updateAngle(entity, 0);
      break;
    case SDLK_DOWN:
      rigidBody.velocity = keyboardControl.downVelocity;
      sprite.srcRect.y = sprite.height * 2;
      updateAngle(entity, 3 * M_PI / 2);
      break;
    case SDLK_LEFT:
      rigidBody.velocity = keyboardControl.leftVelocity;
      sprite.srcRect.y = sprite.height * 3;
      updateAngle(entity, M_PI);
      break;
    }
  }
}

void KeyboardMovementSystem::onKeyboardRelease(KeyReleaseEvent &e) {
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

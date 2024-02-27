#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"

class MovementSystem : public System {
public:
  MovementSystem() {
    // require tranform component
    RequireComponent<RigidBodyComponent>();
    RequireComponent<TransformComponent>();
  }
  void Update(double dt) {
    // update component position every frame
    //
    for (auto enity : GetSystemEntities()) {
      // update entity position based on its velocity
      TransformComponent &transform = enity.GetComponent<TransformComponent>();
      const RigidBodyComponent &rigidBody =
          enity.GetComponent<RigidBodyComponent>();

      transform.position.x += rigidBody.velocity.x * dt;
      transform.position.y += rigidBody.velocity.y * dt;
    }
  }
};

#endif

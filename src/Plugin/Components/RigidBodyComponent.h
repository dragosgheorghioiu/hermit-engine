#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/vec2.hpp>

struct RigidBodyComponent {
  glm::vec2 velocity;
  glm::vec2 maxVelocity;
  glm::vec2 acceleration;
  bool isGrounded;

  RigidBodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0),
                     glm::vec2 acceleration = glm::vec2(0.0, 0.0),
                     glm::vec2 maxVelocity = glm::vec2(0.0, 0.0))
      : velocity(velocity), acceleration(acceleration),
        maxVelocity(maxVelocity), isGrounded(false) {}
};

#endif

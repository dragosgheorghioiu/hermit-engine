#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/vec2.hpp>

struct PluginTransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;

  PluginTransformComponent(glm::vec2 position = glm::vec2(0, 0),
                           glm::vec2 scale = glm::vec2(1.0, 1.0),
                           double rotation = 0.0f)
      : position(position), scale(scale), rotation(rotation) {}
};

#endif

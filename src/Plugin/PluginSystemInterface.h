#ifndef PLUGIN_SYSTEM_INTERFACE_H
#define PLUGIN_SYSTEM_INTERFACE_H

#include "../ECS/ECS.h"

class PluginSystemInterface : public System {
public:
  virtual void Update() = 0;
  virtual ~PluginSystemInterface() = default;
  virtual const std::string GetName() const = 0;
};

#endif

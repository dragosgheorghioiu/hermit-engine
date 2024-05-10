#include "PluginRenderSystem.h"
#include <iostream>

PluginRenderSystem::PluginRenderSystem() {
  std::cout << "PluginRenderSystem constructor" << std::endl;
}

void PluginRenderSystem::update(std::vector<void *> params) {
  // Do something
  std::cout << "PluginRenderSystem update" << std::endl;
}

const std::string PluginRenderSystem::GetName() const { return "RenderSystem"; }

std::function<void(void *)>
PluginRenderSystem::getCallback(std::string eventType) {
  return nullptr;
}

PluginRenderSystem::~PluginRenderSystem() {
  std::cout << "PluginRenderSystem destructor" << std::endl;
}

extern "C" void *createInstance() {
  std::cout << "createInstance for PluginRender System" << std::endl;
  return new PluginRenderSystem();
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<PluginRenderSystem *>(instance);
}

extern "C" const char *getName() { return "PluginRenderSystem"; }

extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"SpriteComponent", "TransformComponent",
                                     nullptr};
  return components;
}

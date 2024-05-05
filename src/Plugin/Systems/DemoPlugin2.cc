#include "DemoPlugin2.h"
#include <iostream>

DemoPlugin2::DemoPlugin2() {
  std::cout << "DemoPlugin2 constructor" << std::endl;
}

const std::string DemoPlugin2::GetName() const { return "DemoPlugin2"; }

void DemoPlugin2::update(std::vector<void *> params) {
  std::vector<EntityType> entities = getSystemEntities();
  for (auto entity : entities) {
    std::cout << "Entity id: " << entity.getId() << std::endl;
    entity.hasComponent("PluginComponent");
  }
}

DemoPlugin2::~DemoPlugin2() {
  std::cout << "DemoPlugin2 destructor" << std::endl;
}

extern "C" void *createInstance() { return new DemoPlugin2(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoPlugin2 *>(instance);
}
extern "C" const char *getName() { return "DemoPlugin2"; }
extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"PluginComponent", nullptr};
  return components;
}

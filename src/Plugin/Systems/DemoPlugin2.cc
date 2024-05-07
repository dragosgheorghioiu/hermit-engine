#include "DemoPlugin2.h"
#include "../Components/DemoComponent.h"
#include "../Events/DemoEvent.h"

DemoPlugin2::DemoPlugin2() {
  // std::cout << "DemoPlugin2 constructor" << std::endl;
}

const std::string DemoPlugin2::GetName() const { return "DemoPlugin2"; }

void DemoPlugin2::update(std::vector<void *> params) {
  std::vector<EntityType> entities = getSystemEntities();
  for (auto entity : entities) {
    DemoComponent *demoComponent = static_cast<DemoComponent *>(
        entity.getComponent("PluginComponent").instance);
    if (demoComponent->value < 50) {
      demoComponent->value += 1;
      Logger::Log("demoComponent value is " +
                  std::to_string(demoComponent->value));
    }
  }
}

DemoPlugin2::~DemoPlugin2() {
  // std::cout << "DemoPlugin2 destructor" << std::endl;
}

// return pointer to member function
std::function<void(void *)> DemoPlugin2::getCallback(std::string eventType) {
  if (eventType == "PluginEvent") {
    return [this](void *event) { this->onPluginEvent(event); };
  }
  return nullptr;
}

void DemoPlugin2::onPluginEvent(void *event) {
  DemoEvent *demoEvent = static_cast<DemoEvent *>(event);
  Logger::Log("DemoPlugin2 received event with value " +
              std::to_string(demoEvent->value));
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

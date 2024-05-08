#include "DemoPlugin.h"
#include "../Components/DemoComponent.h"
#include "../Events/DemoEvent.h"

DemoPlugin::DemoPlugin() {
  // std::cout << "DemoPlugin constructor" << std::endl;
}

const std::string DemoPlugin::GetName() const { return "DemoPlugin"; }

void DemoPlugin::update(std::vector<void *> params) {
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

DemoPlugin::~DemoPlugin() {
  // std::cout << "DemoPlugin destructor" << std::endl;
}

// return pointer to member function
std::function<void(void *)> DemoPlugin::getCallback(std::string eventType) {
  if (eventType == "PluginEvent") {
    return [this](void *event) { this->onPluginEvent(event); };
  }
  return nullptr;
}

void DemoPlugin::onPluginEvent(void *event) {
  DemoEvent *demoEvent = static_cast<DemoEvent *>(event);
  Logger::Log("DemoPlugin received event with value " +
              std::to_string(demoEvent->value));
}

extern "C" void *createInstance() { return new DemoPlugin(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoPlugin *>(instance);
}
extern "C" const char *getName() { return "DemoPlugin"; }
extern "C" const char **getRequiredComponents() {
  static const char *components[] = {"PluginComponent", nullptr};
  return components;
}

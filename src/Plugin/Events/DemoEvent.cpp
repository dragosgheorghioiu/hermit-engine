#include "DemoEvent.h"
#include <any>
#include <string>
#include <vector>

extern "C" void *createInstance(std::vector<std::any> args) {
  int i = std::any_cast<int>(args[0]);
  int j = std::any_cast<int>(args[1]);
  std::string str = std::any_cast<const char *>(args[2]);
  return static_cast<void *>(new DemoEvent(i, j, str));
}
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoEvent *>(instance);
}
extern "C" const char *getEventName() { return "PluginEvent"; }

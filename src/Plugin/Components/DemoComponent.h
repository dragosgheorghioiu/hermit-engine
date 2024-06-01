#ifndef DEMO_COMPONENT_H
#define DEMO_COMPONENT_H

#include <string>
struct DemoComponent {
  int value;
  std::string str;

  DemoComponent(int value = 0, std::string str = "") : value(value), str(str) {}
  ~DemoComponent() {}
};

#endif

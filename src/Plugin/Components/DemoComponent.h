#ifndef DEMO_COMPONENT_H
#define DEMO_COMPONENT_H

#include <iostream>

struct DemoComponent {
  int value;
  std::string str;

  DemoComponent(int value = 0, std::string str = "") : value(value), str(str) {
    std::cout << value << " " << str << std::endl;
  }
  ~DemoComponent() { std::cout << "DemoComponent destructor" << std::endl; }
};

#endif

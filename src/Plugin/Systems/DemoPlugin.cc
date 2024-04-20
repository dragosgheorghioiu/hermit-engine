#include "DemoPlugin.h"
#include <iostream>

DemoPlugin::DemoPlugin() { std::cout << "Created " << GetName() << std::endl; }

const std::string DemoPlugin::GetName() const { return "DemoPlugin"; }

void DemoPlugin::Update(std::vector<void *> params) {
  int *a = static_cast<int *>(params[0]);
  int *b = static_cast<int *>(params[1]);
  std::cout << "DemoPlugin update: "
            << "a = " << *a << ", b = " << *b << std::endl;
  // update a
  *a = *a + 1;
}

DemoPlugin::~DemoPlugin() { std::cout << "DemoPlugin destructor" << std::endl; }

extern "C" void *createInstance() { return new DemoPlugin(); }

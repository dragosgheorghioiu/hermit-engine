#include "DemoPlugin.h"
#include <iostream>

DemoPlugin::DemoPlugin() {
  counter = 0;
  std::cout << "Created " << GetName() << std::endl;
}

const std::string DemoPlugin::GetName() const { return "DemoPlugin"; }

void DemoPlugin::Update() {
  std::cout << "DemoPlugin update: " << counter++ << std::endl;
}

DemoPlugin::~DemoPlugin() { std::cout << "DemoPlugin destructor" << std::endl; }

extern "C" void *createInstance() { return new DemoPlugin(); }

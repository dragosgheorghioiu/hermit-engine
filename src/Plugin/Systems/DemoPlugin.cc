#include "DemoPlugin.h"
#include <iostream>

DemoPlugin::DemoPlugin() { std::cout << "DemoPlugin constructor" << std::endl; }

void DemoPlugin::Update() { std::cout << "DemoPlugin Update" << std::endl; }

DemoPlugin::~DemoPlugin() { std::cout << "DemoPlugin destructor" << std::endl; }

extern "C" void printHello() {
  std::cout << "Hello from the second internal plugin!" << std::endl;
}

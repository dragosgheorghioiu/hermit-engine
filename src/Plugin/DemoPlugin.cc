#include "DemoPlugin.h"
#include <iostream>

DemoPlugin::DemoPlugin() { std::cout << "DemoPlugin constructor" << std::endl; }

void DemoPlugin::Update() { std::cout << "DemoPlugin Update" << std::endl; }

PluginSystemInterface *DemoPlugin::createPlugin() { return new DemoPlugin(); }

DemoPlugin::~DemoPlugin() { std::cout << "DemoPlugin destructor" << std::endl; }

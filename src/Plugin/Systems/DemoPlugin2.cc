#include "DemoPlugin2.h"
#include <iostream>

DemoPlugin2::DemoPlugin2() {
  counter = 0;
  std::cout << "Created " << GetName() << std::endl;
}

const std::string DemoPlugin2::GetName() const { return "DemoPlugin2"; }

void DemoPlugin2::Update() {
  std::cout << "DemoPlugin2 update: " << counter++ << std::endl;
}

DemoPlugin2::~DemoPlugin2() {
  std::cout << "lmaaaoooo destructor" << std::endl;
}

extern "C" void *createInstance() { return new DemoPlugin2(); }

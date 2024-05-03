#include "DemoPlugin2.h"
#include <iostream>

DemoPlugin2::DemoPlugin2() {
  std::cout << "DemoPlugin2 constructor" << std::endl;
}

const std::string DemoPlugin2::GetName() const { return "DemoPlugin2"; }

void DemoPlugin2::Update(std::vector<void *> params) {
  int *i = static_cast<int *>(params[0]);
  // std::cout << "DemoPlugin2 update: " << *i << std::endl;
  (*i) += 2;
}

DemoPlugin2::~DemoPlugin2() {
  std::cout << "DemoPlugin2 destructor" << std::endl;
}

extern "C" void *createInstance() { return new DemoPlugin2(); }
extern "C" void destroyInstance(void *instance) {
  delete static_cast<DemoPlugin2 *>(instance);
}
extern "C" const char *getName() { return "DemoPlugin2"; }

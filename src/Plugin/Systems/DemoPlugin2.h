#ifndef DEMO_PLUGIN_2_H
#define DEMO_PLUGIN_2_H

#include "../SystemInstance.h"
#include <string>

class DemoPlugin2 : public SystemInstance {

public:
  DemoPlugin2();
  void Update() override;
  const std::string GetName() const;
  ~DemoPlugin2() override;
};

#endif

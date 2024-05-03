#ifndef DEMO_PLUGIN_2_H
#define DEMO_PLUGIN_2_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class DemoPlugin2 : public SystemInstance {

public:
  DemoPlugin2();
  void Update(std::vector<void *> params) override;
  const std::string GetName() const;
  ~DemoPlugin2() override;
};

#endif

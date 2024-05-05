#ifndef DEMO_PLUGIN_2_H
#define DEMO_PLUGIN_2_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class DemoPlugin2 : public SystemInstance {
private:
  const char **requiredComponents;

public:
  DemoPlugin2();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  // const char **getRequiredComponents() override;
  ~DemoPlugin2() override;
};

#endif

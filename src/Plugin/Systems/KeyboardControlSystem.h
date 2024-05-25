#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../SystemInstance.h"
#include <string>
#include <vector>

class KeyboardControlSystem : public SystemInstance {
private:
  bool pressedKeys[3];

public:
  KeyboardControlSystem();
  void update(std::vector<void *> params) override;
  const std::string GetName() const;
  std::function<void(void *)> getCallback(std::string eventType) override;
  std::unordered_map<std::string, std::function<void()>>
  getGUIElements() override;
  void demoWindow();
  void onKeyPress(void *event);
  void onKeyRelease(void *event);
  ~KeyboardControlSystem() override;
};

#endif
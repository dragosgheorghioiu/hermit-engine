#ifndef DEMO_EVENT_H
#define DEMO_EVENT_H

#include <string>

struct DemoEvent {
  int value;
  std::string str;

  DemoEvent(int value = 0, std::string str = "") : value(value), str(str) {}
  ~DemoEvent() {}
};

#endif

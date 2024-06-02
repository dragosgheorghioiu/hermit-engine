#ifndef DEMO_EVENT_H
#define DEMO_EVENT_H

#include <string>

struct DemoEvent {
  int value;
  int value2;
  std::string str;

  DemoEvent(int value = 0, int value2 = 0, std::string str = "")
      : value(value), value2(value2), str(str) {}
  ~DemoEvent() {}
};

#endif

#ifndef SYSTEM_INSTANCE_H
#define SYSTEM_INSTANCE_H

class SystemInstance {
public:
  virtual ~SystemInstance() = default;
  virtual void Update() = 0;
};

#endif

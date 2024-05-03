#ifndef SYSTEM_INSTANCE_H
#define SYSTEM_INSTANCE_H

#include <bitset>
#include <vector>

const int MAX_PLUGINS_COMPONENTS = 32;

typedef std::bitset<MAX_PLUGINS_COMPONENTS> Signature;

class SystemInstance {
private:
  Signature componentSignature;
  std::vector<int> entityIDs;

public:
  virtual ~SystemInstance() = default;
  virtual void Update(std::vector<void *> params) = 0;
  void addEntityToSystem(const int &entityId) { entityIDs.push_back(entityId); }
  void removeEntityFromSystem(const int &entityId) {
    // find the entity id and remove it
    for (int i = 0; i < entityIDs.size(); i++) {
      if (entityIDs[i] == entityId) {
        entityIDs.erase(entityIDs.begin() + i);
        break;
      }
    }
  }
  std::vector<int> getSystemEntities() const { return entityIDs; }
  const Signature &getComponentSignature() const { return componentSignature; }
};

#endif

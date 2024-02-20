#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

class Entity {
private:
  int id;

public:
  Entity(int id) : id(id){};
  int GetId() const;

  bool operator==(const Entity &other) const { return id == other.id; }
  bool operator!=(const Entity &other) const { return id != other.id; }
};

struct IComponent {
protected:
  static int nextId;
};

template <typename T> class Component : public IComponent {
  static int GetId() {
    static auto id = nextId++;
    return id;
  }
};

class System {
private:
  Signature componentSignature;
  std::vector<Entity> entities;

public:
  System() = default;
  ~System() = default;

  void AddEntityToSystem(const Entity &entity);
  void RemoveEntityFromSystem(const Entity &entity);
  std::vector<Entity> GetSystemEntites() const;
  const Signature &GetComponentSignature() const;

  template <typename TComponent> void RequireComponent();
};

template <typename TComponent> void System::RequireComponent() {
  const auto componentId = Component<TComponent>::GetId();
  componentSignature.set(componentId);
}

class Registry {};

#endif

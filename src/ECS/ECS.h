#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include <bitset>
#include <memory>
#include <set>
#include <sys/types.h>
#include <typeindex>
#include <unordered_map>
#include <utility>
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
  bool operator<=(const Entity &other) const { return id <= other.id; }
  bool operator>=(const Entity &other) const { return id >= other.id; }
  bool operator<(const Entity &other) const { return id < other.id; }
  bool operator>(const Entity &other) const { return id > other.id; }

  template <typename TComponent, typename... TArgs>
  void AddComponent(TArgs &&...args);
  template <typename TComponent> void RemoveComponent();
  template <typename TComponent> TComponent &GetComponent() const;
  template <typename TComponent> bool HasComponent() const;

  class Registry *registry;
};

struct IComponent {
protected:
  static int nextId;
};

template <typename T> class Component : public IComponent {
public:
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
  std::vector<Entity> GetSystemEntities() const;
  const Signature &GetComponentSignature() const;

  template <typename TComponent> void RequireComponent();
};

template <typename TComponent> void System::RequireComponent() {
  const auto componentId = Component<TComponent>::GetId();
  componentSignature.set(componentId);
}

class IPool {
public:
  virtual ~IPool(){};
};

template <typename T> class Pool : public IPool {
private:
  std::vector<T> data;

public:
  Pool(int n) { data.resize(n); }
  ~Pool() = default;
  bool IsEmpty() { return data.empty(); }
  int GetSize() { return data.size(); }
  void Resize(int n) { data.resize(n); }
  void Clear() { data.clear(); }
  void Add(T component) { data.push_back(component); }
  void Set(int index, T component) { data[index] = component; }
  T &Get(int index) { return data[index]; }
  T &operator[](unsigned int index) { return data[index]; }
};

class Registry {
private:
  int numEntities = 0;

  // sets of entities that are marked to be removed at update
  std::set<Entity> entitiesToBeAdded;
  std::set<Entity> entitiesToBeDestroyed;

  // vector of pools
  // each pool contains all the data for a certain component type
  // componentsPool index is a component id
  // Pool index is a entity id
  std::vector<std::shared_ptr<IPool>> componentsPools;

  // Vector of components signatures per entity
  // Vector index is entity id
  std::vector<Signature> entityComponentSignatures;

  // vector of systems
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

public:
  Registry() { Logger::Log("Registry constructor"); }
  ~Registry() { Logger::Log("Registry destructor"); }

  Entity CreateEntity();
  void AddEntityToSystem(const Entity &entity);
  void DestroyEntity(const Entity &entity);

  template <typename TComponent, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args);
  template <typename TComponent> void RemoveComponent(Entity entity);
  template <typename TComponent> TComponent &GetComponent(Entity entity);
  template <typename TComponent> bool HasComponent(Entity entity) const;

  template <typename TSystem, typename... TArgs>
  void AddSystem(TArgs &&...args);
  template <typename TSystem> void RemoveSystem();
  template <typename TSystem> TSystem &GetSystem();
  template <typename TSystem> bool HasSystem(System system) const;

  void AddEntityToSystems(Entity entity);

  void Update();
};

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs &&...args) {
  std::shared_ptr<TSystem> newSystem(new TSystem(std::forward<TArgs>(args)...));
  systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem> void Registry::RemoveSystem() {
  auto system_key = systems.find(std::type_index(typeid(TSystem)));
  systems.erase(system_key);
}
template <typename TSystem> TSystem &Registry::GetSystem() {
  auto system_key = systems.find(std::type_index(typeid(TSystem)));
  return *(std::static_pointer_cast<TSystem>(system_key->second));
}
template <typename TSystem> bool Registry::HasSystem(System system) const {
  auto system_key = systems.find(std::type_index(typeid(TSystem)));
  return system_key != systems.end();
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs &&...args) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  if (componentId >= static_cast<int>(componentsPools.size())) {
    componentsPools.resize(componentId + 1, nullptr);
  }

  if (componentsPools[componentId] == nullptr) {
    std::shared_ptr<Pool<TComponent>> newPool =
        std::make_shared<Pool<TComponent>>(entityId + 1);
    componentsPools[componentId] = newPool;
  }

  std::shared_ptr<Pool<TComponent>> pool =
      std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);

  if (entityId >= pool->GetSize()) {
    pool->Resize(entityId + 1);
  }

  TComponent newComponent(std::forward<TArgs>(args)...);

  pool->Set(entityId, newComponent);
  entityComponentSignatures[entityId].set(componentId);

  Logger::Log("Component has been added to entity with id = " +
              std::to_string(entityId));
}

template <typename TComponent> void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  entityComponentSignatures[entityId].set(componentId, false);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent &Registry::GetComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  std::shared_ptr<Pool<TComponent>> pool =
      std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);

  return pool->Get(entityId);
}

template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs &&...args) {
  registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent> void Entity::RemoveComponent() {
  registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent> TComponent &Entity::GetComponent() const {
  return registry->GetComponent<TComponent>(*this);
}

template <typename TComponent> bool Entity::HasComponent() const {
  return registry->HasComponent<TComponent>(*this);
}

#endif

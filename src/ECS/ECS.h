#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include "Plugin/PluginComponentFactory.h"
#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <string>
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
  ~Entity() = default;
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

  void addComponent(ComponentFactoryInfo componentInfo);
  void removeComponent(ComponentInfo &componentInfo);
  ComponentInfo &getComponent(ComponentInfo &componentInfo);
  bool hasComponent(ComponentInfo &componentInfo);

  void Kill();
  void Tag(const std::string &tag);
  void RemoveTag();
  bool HasTag(const std::string &tag) const;

  void Group(const std::string &group);
  void RemoveGroup(const std::string &group);
  bool BelongsGroup(const std::string &group) const;

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
  virtual ~IPool() = default;
  virtual void RemoveEntityFromPool(int entityId) = 0;
};

template <typename T> class Pool : public IPool {
private:
  std::vector<T> data;
  int size;

  std::unordered_map<int, int> entityToIndex;
  std::unordered_map<int, int> indexToEntity;

public:
  Pool(int n) {
    size = 0;
    data.resize(n);
  }
  ~Pool() = default;
  void RemoveEntityFromPool(int entityId) {
    if (entityToIndex.find(entityId) == entityToIndex.end())
      return;
    Remove(entityId);
  }
  bool IsEmpty() { return size == 0; }
  int GetSize() { return size; }
  void Resize(int n) { data.resize(n); }
  void Clear() {
    data.clear();
    size = 0;
  }
  void Add(T component) { data.push_back(component); }
  void Set(int entityId, T component) {
    if (entityToIndex.find(entityId) != entityToIndex.end()) {
      int index = entityToIndex[entityId];
      data[index] = component;
    } else {
      // add new component to entity
      int index = size;
      entityToIndex.insert({entityId, index});
      indexToEntity.insert({index, entityId});
      if (index >= static_cast<int>(data.size())) {
        data.resize(size * 2);
      }
      data[index] = component;
      size++;
    }
  }
  void Remove(int entityId) {
    // move last element in position of to be deleted element
    int indexOfRemoved = entityToIndex[entityId];
    int indexOfLast = size - 1;
    data[indexOfRemoved] = data[indexOfLast];

    // update maps
    int entityIdOfLast = indexToEntity[indexOfLast];
    entityToIndex[entityIdOfLast] = indexOfRemoved;
    indexToEntity[indexOfRemoved] = entityIdOfLast;

    // delete last element of maps
    entityToIndex.erase(entityId);
    indexToEntity.erase(indexOfLast);

    size--;
  }
  T &Get(int entityId) {
    int index = entityToIndex[entityId];
    return data[index];
  }
  T &operator[](unsigned int entityId) { return data[entityToIndex[entityId]]; }
};

class ComponentInfoPool {
private:
  std::vector<std::unique_ptr<ComponentInfo>> data;
  int size;

  std::unordered_map<int, int> entityToIndex;
  std::unordered_map<int, int> indexToEntity;

public:
  ComponentInfoPool(int n) {
    size = n;
    data.resize(n);
  }
  ~ComponentInfoPool() = default;
  void RemoveEntityFromPool(int entityId) {
    if (entityToIndex.find(entityId) == entityToIndex.end())
      return;
    Remove(entityId);
  }
  bool IsEmpty() { return size == 0; }
  int GetSize() { return size; }
  void Resize(int n) { data.resize(n); }
  void Clear() {
    data.clear();
    size = 0;
  }
  void Add(std::unique_ptr<ComponentInfo> component) {
    data.push_back(std::move(component));
  }
  void Set(int entityId, std::unique_ptr<ComponentInfo> component) {
    Logger::Log("Set component");
    if (entityToIndex.find(entityId) != entityToIndex.end()) {
      int index = entityToIndex[entityId];
      data[index] = std::move(component);
    } else {
      // add new component to entity
      int index = size;
      entityToIndex.insert({entityId, index});
      indexToEntity.insert({index, entityId});
      if (index >= static_cast<int>(data.size())) {
        data.resize(size * 2);
      }
      data[index] = std::move(component);
      size++;
    }
  }
  void Remove(int entityId) {
    // move last element in position of to be deleted element
    int indexOfRemoved = entityToIndex[entityId];
    int indexOfLast = size - 1;
    data[indexOfRemoved] = std::move(data[indexOfLast]);

    // update maps
    int entityIdOfLast = indexToEntity[indexOfLast];
    entityToIndex[entityIdOfLast] = indexOfRemoved;
    indexToEntity[indexOfRemoved] = entityIdOfLast;

    // delete last element of maps
    entityToIndex.erase(entityId);
    indexToEntity.erase(indexOfLast);
  }
  ComponentInfo &Get(int entityId) {
    int index = entityToIndex[entityId];
    return *data[index];
  }
  ComponentInfo &operator[](unsigned int entityId) {
    return *data[entityToIndex[entityId]];
  }
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
  std::vector<std::shared_ptr<ComponentInfoPool>> pluginComponentPools;

  // Vector of components signatures per entity
  // Vector index is entity id
  std::vector<Signature> entityComponentSignatures;

  // vector of systems
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

  // deque of freeIds
  std::deque<int> freeIds;

  // Entity tags duplicated for fast access both ways
  std::unordered_map<std::string, Entity> entityPerTag;
  std::unordered_map<int, std::string> tagPerEntity;

  // Entity groups duplicated for fast access both ways
  std::unordered_map<std::string, std::set<Entity>> entitiesPerGroup;
  std::unordered_map<int, std::string> groupPerEntity;

public:
  Registry() { Logger::Log("Registry constructor"); }
  ~Registry() { Logger::Log("Registry destructor"); }

  Entity CreateEntity();
  void AddEntityToBeDestroyed(const Entity &entity);
  void DestroyEntity(const Entity &entity);

  // Tags
  void AddTagToEntity(Entity entity, const std::string &tag);
  bool EntityHasTag(Entity entity, const std::string &tag) const;
  Entity GetEntityByTag(const std::string &tag) const;
  void RemoveTagFromEntity(Entity entity);

  // Groups
  void AddGroupToEntity(Entity entity, const std::string &group);
  bool EntityBelongsGroup(Entity entity, const std::string &group) const;
  std::set<Entity> GetEntitiesByGroup(const std::string &group) const;
  void RemoveEntityFromGroup(Entity entity, const std::string &group);

  template <typename TComponent, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args);
  template <typename TComponent> void RemoveComponent(Entity entity);
  template <typename TComponent> TComponent &GetComponent(Entity entity);
  template <typename TComponent> bool HasComponent(Entity entity) const;

  void addComponentToEntity(const Entity &entity,
                            ComponentFactoryInfo &componentInfo);
  void removeComponentFromEntity(const Entity &entity,
                                 ComponentInfo &componentInfo);
  ComponentInfo &getComponentFromEntity(const Entity &entity,
                                        ComponentInfo &componentInfo);
  bool hasComponentFromEntity(const Entity &entity,
                              ComponentInfo &componentInfo);

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

  TComponent newComponent(std::forward<TArgs>(args)...);

  pool->Set(entityId, newComponent);
  entityComponentSignatures[entityId].set(componentId);

  // Logger::Log("Component has been added to entity with id = " +
  //             std::to_string(entityId));
}

template <typename TComponent> void Registry::RemoveComponent(Entity entity) {
  const auto componentId = Component<TComponent>::GetId();
  const auto entityId = entity.GetId();

  std::shared_ptr<Pool<TComponent>> pool =
      std::static_pointer_cast<Pool<TComponent>>(componentsPools[componentId]);
  pool->Remove(entityId);
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

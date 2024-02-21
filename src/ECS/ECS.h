#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <set>
#include <sys/types.h>
#include <typeindex>
#include <unordered_map>
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
  void Resize(int n) { data.resize(); }
  void Clear() { data.clear(); }
  void Add(T component) { data.push_back(component); }
  void Set(int index, T component) { data[index] = component; }
  T &Get(int index) { return static_cast<T>(data[index]); }
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
  std::vector<IPool *> componentsPools;

  // Vector of components signatures per entity
  // Vector index is entity id
  std::vector<Signature> entityComponentSignatures;

  // vector of systems
  std::unordered_map<std::type_index, System *> systems;

public:
  Registry() = default;

  Entity CreateEntity();
  void AddEntityToSystem(const Entity &entity);
  void DestroyEntity(const Entity &entity);

  template <typename T, typename... TArgs>
  void AddComponent(Entity entity, TArgs &&...args);
};

#endif

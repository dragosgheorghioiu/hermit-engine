#ifndef COMPONENT_INFO_POOL_H
#define COMPONENT_INFO_POOL_H

#include "../Plugin/PluginComponentFactory.h"
#include <memory>

class ComponentInfoPool {
private:
  std::vector<std::unique_ptr<ComponentInfo>> data;
  int size;
  std::string name;

  std::unordered_map<int, int> entityToIndex;
  std::unordered_map<int, int> indexToEntity;

public:
  ComponentInfoPool(int n, std::string name) : size(n), name(name) {
    data.resize(n);
  }
  ~ComponentInfoPool() = default;
  std::string GetName() { return name; }
  void RemoveEntityFromPool(int entityId) {
    if (entityToIndex.find(entityId) == entityToIndex.end())
      return;
    Remove(entityId);
  }
  bool Has(int entityId) {
    if (entityToIndex.find(entityId) == entityToIndex.end())
      return false;
    return true;
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

#endif

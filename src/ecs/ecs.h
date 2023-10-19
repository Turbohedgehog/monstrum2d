#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <bitset>
#include <unordered_map>

#include "common/common_types.h"

namespace m2d {

namespace ecs {

class ECS : public std::enable_shared_from_this<ECS> {
 public:
  ECS(const std::string& name, HolderWeakPtr ecs_holder);
  EntityWeakPtr CreateEnity(const std::vector<StringIndex>& components = {});
  HolderWeakPtr GetHolder() const;
  PoolWeakPtr GetPool() const;
  FilterWeakPtr GetOrCreateFilter(const std::vector<StringIndex>& components);
  void Tick(float delta);
  void RemoveEntity(std::size_t enity_id);

 private:
  std::string name_;
  PoolPtr pool_;
  HolderWeakPtr ecs_holder_;
  std::size_t entity_counter_ = 0;
  std::map<std::size_t, EntityPtr> enities_;
  std::unordered_map<ComponentBitmask, FilterPtr> filters_;
};

}  // namespace ecs

}  // namespace m2d

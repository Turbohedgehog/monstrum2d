#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "common/common_types.h"

namespace m2d {

namespace ecs {

class ECS : public std::enable_shared_from_this<ECS> {
 public:
  ECS(const std::string& name, HolderWeakPtr ecs_holder);
  EntityWeakPtr CreateEnity(const std::vector<std::string>& components = {});
  HolderWeakPtr GetHolder() const;
  PoolWeakPtr GetPool() const;
  void Tick(float delta);

 private:
  std::string name_;
  PoolPtr pool_;
  HolderWeakPtr ecs_holder_;
  std::size_t entity_counter_ = 0;
  std::map<std::size_t, EntityPtr> enities_;
};

}  // namespace ecs

}  // namespace m2d

#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Entity : public std::enable_shared_from_this<Entity> {
 public:
  Entity(std::size_t id, ECSWeakPtr ecs);

  std::size_t GetId() const;
  ECSWeakPtr GetECS() const;
  void AddComponents(const std::vector<std::string>& components = {});

  ComponentWeakPtr GetComponent(const StringIndex& index) const;
  bool Tick(float delta);

 private:
  std::size_t id_;
  ECSWeakPtr ecs_;
  std::map<std::size_t, ComponentPtr> components_;
};

}  // namespace ecs

}  // namespace m2d

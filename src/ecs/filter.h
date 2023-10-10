#pragma once

#include <vector>
#include <map>
#include <set>

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Filter {
 public:
  Filter(ECSWeakPtr ecs, const ComponentBitmask& filter_bitmask);
  bool IsValid() const;
  void ProcessEntity(EntityWeakPtr entity);

 private:
  ECSWeakPtr ecs_;
  ComponentBitmask filter_bitmask_;
  std::map<std::size_t, EntityWeakPtr> entities_;

};

}  // namespace ecs

}  // namespace m2d

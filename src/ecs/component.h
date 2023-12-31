#pragma once

#include <optional>

#include "common/common_types.h"

namespace m2d {

namespace ecs {
  
class Component {
 public:
  Component(std::size_t type_id, std::size_t enity_id, ComponentSchemaWeakPtr schema, ComponentDataPtr data);

  ComponentDataPtr GetData() const;
  bool Tick(float delta);

  ComponentSchemaWeakPtr GetSchema() const;

 private:
  std::size_t type_id_ = 0;
  std::size_t enity_id_ = 0;
  ComponentSchemaWeakPtr schema_;
  ComponentDataPtr data_;
  std::optional<ComponentLifetime> lifetime_;
  
};

}  // namespace ecs

}  // namespace m2d

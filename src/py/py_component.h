#pragma once

#include "py/py.h"

#include "common/common_types.h"

namespace m2d {

namespace py {

class Component {
 public:
  static bp::object CreateClassDeclaration();

  void SetComponent(ecs::ComponentWeakPtr component);
  ecs::ComponentWeakPtr GetComponent() const;
  void SetField(bp::object field_path, bp::object value);
  bp::object GetField(bp::object field_path);

 private:
  ecs::ComponentDataPtr AccessField(bp::object field_path) const;

  ecs::ComponentWeakPtr component_;
};

}  // namespace py

}  // namespace m2d
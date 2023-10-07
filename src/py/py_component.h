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

 private:
  ecs::ComponentWeakPtr component_;
};

}  // namespace py

}  // namespace m2d
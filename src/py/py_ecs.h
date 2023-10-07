#pragma once

#include "py/py.h"

#include "common/common_types.h"

#include "py/py_entity.h"

namespace m2d {

namespace py {

class ECS {
 public:
  static bp::object CreateClassDeclaration();

  void SetECS(ecs::ECSWeakPtr ecs);
  Entity CreateEntity(bp::list component_names);

 private:
  ecs::ECSWeakPtr ecs_;
};

}  // namespace py

}  // namespace m2d
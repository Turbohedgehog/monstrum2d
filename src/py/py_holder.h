#pragma once

#include <map>
#include <string>

#include "common/common_types.h"

#include "py/py.h"
#include "py/py_ecs.h"

namespace m2d {

namespace py {

class Holder {
 public:
  static bp::object CreateClassDeclaration();

  void SetHolderPtr(ecs::HolderWeakPtr holder);
  void Shutdown();
  ECS GetOrCreateECS(const std::string& ecs_name);

 private:
  ecs::HolderWeakPtr holder_;

};

}  // namespace py

}  // namespace m2d
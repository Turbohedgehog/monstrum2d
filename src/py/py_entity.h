#pragma once

#include <boost/optional.hpp>

#include "common/common_types.h"

#include "py/py.h"
#include "py/py_component.h"

namespace m2d {

namespace py {

class Entity {
 public:
  static bp::object CreateClassDeclaration();

  void SetEntity(ecs::EntityWeakPtr entity);

  bp::object GetComponent(bp::object index) const;
  int GetId() const;

 private:
  ecs::EntityWeakPtr entity_;
};

}  // namespace py

}  // namespace m2d
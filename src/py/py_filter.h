#pragma once

#include "py/py.h"
#include "py/py_entity.h"

#include "common/common_types.h"

namespace m2d {

namespace py {

class Filter;

class Filter {
 public:
  static bp::object CreateClassDeclaration();

  void SetFilter(ecs::FilterWeakPtr filter);
  Filter Iter();
  Entity Next();

 private:
  ecs::FilterWeakPtr filter_;
  ecs::EntityMap::const_iterator entity_iterator_;
};

}  // namespace py

}  // namespace m2d
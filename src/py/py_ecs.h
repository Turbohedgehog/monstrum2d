#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

#include "py/py_entity.h"

namespace bp = boost::python;

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
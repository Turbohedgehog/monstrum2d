#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class ECS {
 public:
  void SetECS(ecs::ECSWeakPtr ecs);

  static bp::object CreateClassDeclaration();

 private:
  ecs::ECSWeakPtr ecs_;
};

}  // namespace py

}  // namespace m2d
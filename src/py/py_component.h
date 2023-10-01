#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class Component {
 public:
  static bp::object CreateClassDeclaration();

  void SetComponent(ecs::ComponentWeakPtr component);

 private:
  ecs::ComponentWeakPtr component_;
};

}  // namespace py

}  // namespace m2d
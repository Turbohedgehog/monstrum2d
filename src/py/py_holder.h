#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

#include "py/py_ecs.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class Holder {
 public:
  void SetHolderPtr(ecs::HolderWeakPtr holder);
  void Shutdown();
  ECS GetOrCreateECS(const std::string& ecs_name);

  static bp::object CreateClassDeclaration();

 private:
  ecs::HolderWeakPtr holder_;

};

}  // namespace py

}  // namespace m2d
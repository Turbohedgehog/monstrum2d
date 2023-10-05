#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>
#include <boost/optional.hpp>

#include "common/common_types.h"

#include "py/py_component.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class Entity {
 public:
  static bp::object CreateClassDeclaration();

  void SetEntity(ecs::EntityWeakPtr entity);

  bp::object GetComponent(bp::object index) const;
  //bp::list GetComponents(bp::list indexes) const;

 private:
  ecs::EntityWeakPtr entity_;


};

}  // namespace py

}  // namespace m2d
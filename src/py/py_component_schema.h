#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

#include "py/py_component.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class ComponentSchema {
 public:
  static bp::object GetClassDeclaration();

  void SetSchema(ecs::ComponentSchemaWeakPtr schema);
  bool Expired() const;
  bp::object AccessField(Component component, bp::tuple fields);

 private:
  ecs::ComponentSchemaWeakPtr schema_;
};

}  // namespace py

}  // namespace m2d

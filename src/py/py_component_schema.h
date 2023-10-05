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
  static bp::object CreateClassDeclaration();

  void SetSchema(ecs::ComponentSchemaWeakPtr schema);
  bool Expired() const;
  ecs::ComponentDataPtr AccessField(Component component, bp::tuple fields) const;
  void SetField(Component component, bp::tuple fields, bp::object value);
  bp::object GetField(Component component, bp::tuple fields);

 private:
  ecs::ComponentSchemaWeakPtr schema_;
};

}  // namespace py

}  // namespace m2d

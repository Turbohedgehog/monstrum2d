#pragma once

#include "py/py.h"

#include "common/common_types.h"

#include "py/py_component.h"

namespace m2d {

namespace py {

class ComponentSchema {
 public:
  static bp::object CreateClassDeclaration();

  void SetSchema(ecs::ComponentSchemaWeakPtr schema);
  bool Expired() const;
  ecs::ComponentDataPtr AccessField(Component component, bp::object field_path) const;
  void SetField(Component component, bp::object field_path, bp::object value);
  bp::object GetField(Component component, bp::object field_path);

 private:
  ecs::ComponentSchemaWeakPtr schema_;
};

}  // namespace py

}  // namespace m2d

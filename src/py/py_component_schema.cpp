#include "py/py_component_schema.h"

#include "ecs/component_shema.h"

namespace m2d {

namespace py {

bp::object ComponentSchema::GetClassDeclaration() {
  static bp::object schema_class = bp::class_<ComponentSchema>("ComponentSchema")
  ;

  return schema_class;
}

void ComponentSchema::SetSchema(ecs::ComponentSchemaWeakPtr schema) {
  schema_ = schema;
}

bool ComponentSchema::Expired() const {
  return schema_.expired();
}

bp::object ComponentSchema::AccessField(Component component, bp::tuple fields) {
  auto len = bp::len(fields);
  for (decltype(len) i = 0; i < len; ++i) {

  }
  return bp::object();
}

}  // namespace py

}  // namespace m2d

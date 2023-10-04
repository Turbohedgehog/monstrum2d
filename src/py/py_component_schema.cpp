#include "py/py_component_schema.h"

#include "ecs/component_schema.h"
#include "ecs/component_data.h"

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
  ecs::FieldIndexContainer index_container;
  for (decltype(len) i = 0; i < len; ++i) {
    auto elem = fields[i];
    bp::extract<std::string> get_string(elem);
    if (get_string.check()) {
      index_container.push_back(get_string());
    } else {
      index_container.push_back(bp::extract<std::size_t>(elem));
    }
  }

  auto data = schema_.lock()->AccessToComponentData(component.GetComponent().lock(), index_container);
  auto data_type = data->GetType();
  switch (data_type) {
    case ecs::ComponentFiledType::Int:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<int>>(data)->data);

    case ecs::ComponentFiledType::Double:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<double>>(data)->data);

    case ecs::ComponentFiledType::String:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<std::string>>(data)->data);
  }
  return bp::object();
}

}  // namespace py

}  // namespace m2d

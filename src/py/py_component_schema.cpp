#include "py/py_component_schema.h"

#include "ecs/component_schema.h"
#include "ecs/component_data.h"

namespace m2d {

namespace py {

bp::object ComponentSchema::CreateClassDeclaration() {
  return bp::class_<ComponentSchema>("ComponentSchema")
      .def("set_field", &ComponentSchema::SetField, bp::args(("component"), ("fields"), ("value")))
      .def("get_field", &ComponentSchema::GetField, bp::args(("component"), ("fields")))
  ;
}

void ComponentSchema::SetSchema(ecs::ComponentSchemaWeakPtr schema) {
  schema_ = schema;
}

bool ComponentSchema::Expired() const {
  return schema_.expired();
}

ecs::ComponentDataPtr ComponentSchema::AccessField(Component component, bp::tuple fields) const {
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

  return schema_.lock()->AccessToComponentData(component.GetComponent().lock(), index_container);

  /*
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
  */
}

void ComponentSchema::SetField(Component component, bp::tuple fields, bp::object value) {
  auto component_data = AccessField(component, fields);
  if (!component_data) {
    return;
  }

  // ToDo: simplify
  auto data_type = component_data->GetType();
  switch (data_type) {
    case ecs::ComponentFiledType::Int:
      std::static_pointer_cast<ecs::ComponentPrimitiveData<int>>(component_data)->data = bp::extract<int>(value);
      break;

    case ecs::ComponentFiledType::Double:
      std::static_pointer_cast<ecs::ComponentPrimitiveData<double>>(component_data)->data = bp::extract<double>(value);
      break;

    case ecs::ComponentFiledType::String:
      std::static_pointer_cast<ecs::ComponentPrimitiveData<std::string>>(component_data)->data = bp::extract<std::string>(value);
      break;
  }
}

bp::object ComponentSchema::GetField(Component component, bp::tuple fields) {
  auto component_data = AccessField(component, fields);
  if (!component_data) {
    return bp::object();
  }

  // ToDo: simplify
  auto data_type = component_data->GetType();
  switch (data_type) {
    case ecs::ComponentFiledType::Int:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<int>>(component_data)->data);

    case ecs::ComponentFiledType::Double:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<double>>(component_data)->data);

    case ecs::ComponentFiledType::String:
      return bp::object(std::static_pointer_cast<ecs::ComponentPrimitiveData<std::string>>(component_data)->data);
  }

  return bp::object();
}

}  // namespace py

}  // namespace m2d

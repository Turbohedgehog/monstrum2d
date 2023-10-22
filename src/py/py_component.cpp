#include "py/py_component.h"

#include "py/py_array.h"

#include "ecs/component_schema.h"
#include "ecs/component_data.h"
#include "ecs/component.h"

namespace m2d {

namespace py {

bp::object Component::CreateClassDeclaration() {
  return bp::class_<Component>("Component")
      .def("set_field", &Component::SetField, (bp::arg("field_path"), bp::arg("value")))
      .def("get_field", &Component::GetField, (bp::arg("field_path")))
  ;
}

void Component::SetComponent(ecs::ComponentWeakPtr component) {
  component_ = component;
}

ecs::ComponentWeakPtr Component::GetComponent() const {
  return component_;
}

ecs::ComponentDataPtr Component::AccessField(bp::object field_path) const {
  auto object_to_index = [](bp::object obj) -> ecs::StringIndex {
    bp::extract<std::string> get_string(obj);
    return get_string.check() ?
        ecs::StringIndex(get_string()) :
        ecs::StringIndex(bp::extract<std::size_t>(obj));
  };
  
  ecs::FieldIndexContainer index_container;
  bp::extract<bp::list> get_list(field_path);
  if (get_list.check()) {
    auto field_path_list = get_list();
    auto len = bp::len(field_path_list);
    for (decltype(len) i = 0; i < len; ++i) {
      auto elem = field_path_list[i];
      index_container.push_back(object_to_index(elem));
    }
  } else {
    index_container.push_back(object_to_index(field_path));
  }

  auto component_ptr = component_.lock();
  auto schema_ptr = component_ptr->GetSchema().lock();

  return schema_ptr->AccessToComponentData(component_ptr, index_container);
}

void Component::SetField(bp::object field_path, bp::object value) {
  auto component_data = AccessField(field_path);
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

bp::object Component::GetField(bp::object field_path) {
  auto component_data = AccessField(field_path);
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

    case ecs::ComponentFiledType::Array:
      Array array;
      array.SetData(std::static_pointer_cast<ecs::ArrayComponentData>(component_data));

      return bp::object(array);
  }

  return bp::object();
}

}  // namespace py

}  // namespace m2d
#include "py/py_holder.h"

#include "ecs/holder.h"

namespace m2d {

namespace py {

bp::object Holder::CreateClassDeclaration() {
  return bp::class_<Holder>("Holder")
      .def("get_or_create_ecs", &Holder::GetOrCreateECS, bp::args(("ecs_name")))
      .def("shutdown", &Holder::Shutdown)
      //.def("get_component_schema", &Holder::GetComponentSchema, bp::args(("schema_name")))
  ;
}

void Holder::SetHolderPtr(ecs::HolderWeakPtr holder) {
  holder_ = holder;
}

void Holder::Shutdown() {
  holder_.lock()->Shutdown();
}

ECS Holder::GetOrCreateECS(const std::string& ecs_name) {
  auto ecs = ECS();
  auto ecs_ptr = holder_.lock()->GetOrCreateECS(ecs_name);
  ecs.SetECS(ecs_ptr);

  return ecs;
}

#if 0
//boost::optional<int> Holder::GetComponentSchemaId(const std::string& schema_name) {
bp::object Holder::GetComponentSchemaId(const std::string& schema_name) {
  auto id = holder_.lock()->GetComponentSchemaIdByName(schema_name);
  return id ? bp::object(static_cast<int>(id.value())) : bp::object();
}

bp::object Holder::GetComponentSchema(const std::string& schema_name) {
  auto it = component_schemas_.find(schema_name);
  
  if (it != component_schemas_.end()) {
    if (it->second.Expired()) {
      component_schemas_.erase(it);
    } else {
      return bp::object(it->second);
    }
  }

  auto component_schema_ptr = holder_.lock()->GetComponentSchema(schema_name);
  if (component_schema_ptr.expired()) {
    //return boost::optional<ComponentSchema>();
    return bp::object();
  }

  ComponentSchema component_schema;
  component_schema.SetSchema(component_schema_ptr);


  component_schemas_[schema_name] = component_schema;

  return bp::object(component_schema);
}
#endif

}  // namespace py

}  // namespace m2d
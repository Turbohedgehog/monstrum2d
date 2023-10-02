#include "py/py_entity.h"

namespace m2d {

namespace py {

bp::object Entity::CreateClassDeclaration() {
  return bp::class_<Entity>("Entity")
      .def("get_component", &Entity::GetComponent, bp::args(("component_name")))
      .def("get_component_by_id", &Entity::GetComponentById, bp::args(("component_id")))
  ;
}

void Entity::SetEntity(ecs::EntityWeakPtr entity) {
  entity_ = entity;
}

boost::optional<Component> Entity::GetComponent(const std::string& component_name) {
  return boost::optional<Component>();
}

boost::optional<Component> Entity::GetComponentById(int component_id) {
  return boost::optional<Component>();
}

}  // namespace py

}  // namespace m2d
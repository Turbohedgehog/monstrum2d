#include "py/py_entity.h"

#include "py/py_component.h"

#include "ecs/entity.h"

namespace m2d {

namespace py {

bp::object Entity::CreateClassDeclaration() {
  return bp::class_<Entity>("Entity")
      .def("get_component", &Entity::GetComponent, bp::args(("component_index")))
      .def("get_id", &Entity::GetId)
  ;
}

void Entity::SetEntity(ecs::EntityWeakPtr entity) {
  entity_ = entity;
}

bp::object Entity::GetComponent(bp::object index) const {
  bp::extract<std::string> get_string(index);
  auto component_ptr = get_string.check() ? 
      entity_.lock()->GetComponent(get_string()) :
      entity_.lock()->GetComponent(bp::extract<std::size_t>(index));

  if (component_ptr.expired()) {
    return bp::object();
  }

  Component component;
  component.SetComponent(component_ptr);

  return bp::object(component);
}

int Entity::GetId() const {
  return static_cast<int>(entity_.lock()->GetId());
}

}  // namespace py

}  // namespace m2d
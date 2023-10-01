#include "py/py_component.h"

namespace m2d {

namespace py {

bp::object Component::CreateClassDeclaration() {
  return bp::class_<Component>("Component")
  ;
}

void Component::SetComponent(ecs::ComponentWeakPtr component) {
  component_ = component;
}

ecs::ComponentWeakPtr Component::GetComponent() const {
  return component_;
}

}  // namespace py

}  // namespace m2d
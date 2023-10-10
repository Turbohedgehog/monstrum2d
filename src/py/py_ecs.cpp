#include "py/py_ecs.h"

#include <algorithm>

#include "py/py_entity.h"

#include "ecs/ecs.h"


namespace m2d {

namespace py {

void ECS::SetECS(ecs::ECSWeakPtr ecs) {
  ecs_ = ecs;
}

bp::object ECS::CreateClassDeclaration() {
  return bp::class_<ECS>("ECS")
      .def("create_entity", &ECS::CreateEntity, bp::args(("component_names")))
  ;
}

Entity ECS::CreateEntity(bp::list component_names) {
  std::vector<ecs::StringIndex> c_names;
  auto len = bp::len(component_names);
  c_names.reserve(len);

  for (decltype(len) i = 0; i < len; ++i) {
    bp::extract<std::string> get_string(component_names[i]);
    if (get_string.check()) {
      c_names.push_back(get_string());
    } else {
      c_names.push_back(bp::extract<std::size_t>(component_names[i]));
    }
  }

  auto entity_ptr = ecs_.lock()->CreateEnity(c_names);
  auto entity = Entity();
  entity.SetEntity(entity_ptr);

  return entity;
}

}  // namespace py

}  // namespace m2d

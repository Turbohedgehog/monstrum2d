#include "py/py_ecs.h"

//#include <algorithm>

#include "py/py_entity.h"

#include "ecs/ecs.h"


namespace m2d {

namespace py {

std::vector<ecs::StringIndex> ExtractComponentIndices(bp::list component_names) {
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

  return c_names;
}

void ECS::SetECS(ecs::ECSWeakPtr ecs) {
  ecs_ = ecs;
}

bp::object ECS::CreateClassDeclaration() {
  return bp::class_<ECS>("ECS")
      .def("create_entity", &ECS::CreateEntity, bp::args(("component_names")))
      .def("remove_entity", &ECS::RemoveEntity, bp::args(("entity_id")))
      .def("get_or_create_filter", &ECS::GetOrCreateFilter, bp::args(("component_names")))
  ;
}

Entity ECS::CreateEntity(bp::list component_names) {
  auto c_names = ExtractComponentIndices(component_names);
  auto entity_ptr = ecs_.lock()->CreateEnity(c_names);
  auto entity = Entity();
  entity.SetEntity(entity_ptr);

  return entity;
}

bp::object ECS::GetOrCreateFilter(bp::list component_names) {
  auto c_names = ExtractComponentIndices(component_names);
  auto filter_ptr = ecs_.lock()->GetOrCreateFilter(c_names);
  if (filter_ptr.expired()) {
    return bp::object();
  }

  Filter filter;
  filter.SetFilter(filter_ptr);

  return bp::object(filter);
}

void ECS::RemoveEntity(int entity_id) {
  if (entity_id < 0) {
    return;
  }

  ecs_.lock()->RemoveEntity(static_cast<std::size_t>(entity_id));
}

}  // namespace py

}  // namespace m2d

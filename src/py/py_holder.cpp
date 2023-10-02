#include "py/py_holder.h"

#include "ecs/holder.h"

namespace m2d {

namespace py {

bp::object Holder::CreateClassDeclaration() {
  return bp::class_<ECS>("Holder")
      .def("get_or_create_ecs", &Holder::GetOrCreateECS, bp::args(("ecs_name")))
      .def("shutdown", &Holder::Shutdown)
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

}  // namespace py

}  // namespace m2d
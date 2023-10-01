#include "py/ecs_wrapper.h"


namespace m2d {

namespace py {

void ECS::SetECS(ecs::ECSWeakPtr ecs) {
  ecs_ = ecs;
}

bp::object ECS::CreateClassDeclaration() {
  return bp::class_<ECS>("ECS");
}

}  // namespace py

}  // namespace m2d

#include "py/py_filter.h"

#include "ecs/entity.h"

namespace m2d {

namespace py {

bp::object Filter::CreateClassDeclaration() {
  return bp::class_<Filter>("Filter")
  ;
}

}  // namespace py

}  // namespace m2d
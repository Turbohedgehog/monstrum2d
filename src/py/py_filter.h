#pragma once

#include "py/py.h"

#include "common/common_types.h"

namespace m2d {

namespace py {

class Filter {
 public:
  static bp::object CreateClassDeclaration();
};

}  // namespace py

}  // namespace m2d
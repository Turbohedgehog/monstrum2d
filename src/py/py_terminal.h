#pragma once

#include "py/py.h"

#include "py/py_screen.h"

namespace m2d {

namespace py {

class Terminal {
 public:
  static bp::object CreateClassDeclaration();
  static Screen CreateScreen();
  static bp::object GetScreen(int id);

};

}  // namespace py

}  // namespace m2d

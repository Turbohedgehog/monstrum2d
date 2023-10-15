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
  static void SetSize(int width, int height);
  static int GetWidth();
  static int GetHeight();

};

}  // namespace py

}  // namespace m2d

#pragma once

#include "py/py.h"

#include "py/py_screen.h"
#include "py/py_vector2d.h"

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
  static IntVector2D GetSize();

};

}  // namespace py

}  // namespace m2d

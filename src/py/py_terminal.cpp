#include "py/py_terminal.h"

#include "terminal/terminal.h"

namespace m2d {

namespace py {

bp::object Terminal::CreateClassDeclaration() {
  static bp::object class_decl =  bp::class_<m2d::py::Terminal>("Terminal")
      .def("create_screen", &Terminal::CreateScreen)//.staticmethod("create_screen")
  ;

  return class_decl;
}

Screen Terminal::CreateScreen() {
  Screen screen;
  screen.SetScreen(hi::Terminal::GetInstance().AppendScreen());

  return screen;
}

}  // namespace py

}  // namespace m2d
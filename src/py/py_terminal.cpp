#include "py/py_terminal.h"

#include "terminal/terminal.h"

namespace m2d {

namespace py {

bp::object Terminal::CreateClassDeclaration() {
  static bp::object class_decl =  bp::class_<m2d::py::Terminal>("Terminal")
      .def("create_screen", &Terminal::CreateScreen).staticmethod("create_screen")
  ;

  return class_decl;
}

Screen Terminal::CreateScreen() {
  Screen screen;
  screen.SetScreen(hi::Terminal::GetInstance().AppendScreen());

  return screen;
}

bp::object Terminal::GetScreen(int id) {
  if (id < 0) {
    return bp::object();
  }

  auto screen_id = static_cast<std::size_t>(id);
  auto screen_ptr = hi::Terminal::GetInstance().GetScreen(screen_id);
  if (screen_ptr.expired()) {
    return bp::object();
  }

  Screen screen;
  screen.SetScreen(screen_ptr);

  return bp::object(screen);
}

}  // namespace py

}  // namespace m2d
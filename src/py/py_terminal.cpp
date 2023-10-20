#include "py/py_terminal.h"

#include "terminal/terminal.h"

namespace m2d {

namespace py {

bp::object Terminal::CreateClassDeclaration() {
  static bp::object class_decl =  bp::class_<m2d::py::Terminal>("Terminal")
      .def("create_screen", &Terminal::CreateScreen).staticmethod("create_screen")
      .def("get_screen", &Terminal::GetScreen).staticmethod("get_screen")
      .def("set_size", &Terminal::SetSize, bp::args(("width"), ("height"))).staticmethod("set_size")
      .def("get_width", &Terminal::GetWidth).staticmethod("get_width")
      .def("get_height", &Terminal::GetHeight).staticmethod("get_height")
      .def("get_size", &Terminal::GetSize).staticmethod("get_size")
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

void Terminal::SetSize(int width, int height) {
  hi::Terminal::GetInstance().SetSize(width, height);
}

int Terminal::GetWidth() {
  return hi::Terminal::GetInstance().GetWidth();
}

int Terminal::GetHeight() {
  return hi::Terminal::GetInstance().GetHeight();
}

IntVector2D Terminal::GetSize() {
  auto& terminal = hi::Terminal::GetInstance();

  return IntVector2D(terminal.GetWidth(), terminal.GetHeight());
}

}  // namespace py

}  // namespace m2d
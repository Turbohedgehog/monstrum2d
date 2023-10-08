#include "py/py_screen.h"

#include "terminal/screen.h"

namespace m2d {

namespace py {

bp::object Screen::CreateClassDeclaration() {
  return bp::class_<m2d::py::Screen>("Screen")
      .def("get_key_pressed", &Screen::GetKeyPressed)
  ;
}

void Screen::SetScreen(hi::ScreenWeakPtr screen) {
  screen_ = screen;
}

bp::object Screen::GetKeyPressed() const {
  if (screen_.expired()) {
    return bp::object();
  }

  auto key_pressed = screen_.lock()->GetKeyPressed();
  return key_pressed ? bp::object(key_pressed.value()) : bp::object();
}

}  // namespace py

}  // namespace m2d
#include "py/py_screen.h"

#include "terminal/screen.h"

namespace m2d {

namespace py {

bp::object Screen::CreateClassDeclaration() {
  return bp::class_<m2d::py::Screen>("Screen")
      .def("get_id", &Screen::GetId)
      .def("get_key_pressed", &Screen::GetKeyPressed)
      .def("clear", &Screen::Clear)
      .def("set_color_pair", &Screen::SetColorPair, bp::args(("pair_id"), ("foreground"), ("background")))
      .def("set_clear_color", &Screen::SetClearColorPair, bp::args(("pair_id")))
      .def("select_color_pair", &Screen::SelectColorPair, bp::args(("pair_id")))
      .def("move_to", &Screen::MoveTo, bp::args(("x"), ("y")))
      .def("print", &Screen::PrintW, bp::args(("str")))
      .def("refresh", &Screen::Refresh)
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

void Screen::SetColorPair(int pair_id, int foreground, int background) {
  if (pair_id < 0 || pair_id > 255) {
    return;
  }

  if (foreground < 0 || foreground > 255) {
    return;
  }

  if (background < 0 || background > 255) {
    return;
  }

  auto p = static_cast<uint8_t>(pair_id);
  auto f = static_cast<uint8_t>(foreground);
  auto b = static_cast<uint8_t>(background);

  screen_.lock()->SetColorPair(p, f, b);
}

void Screen::SetClearColorPair(int pair_id) {
  if (pair_id < 0 || pair_id > 255) {
    return;
  }

  auto p = static_cast<uint8_t>(pair_id);

  screen_.lock()->SetClearColorPair(p);
}

int Screen::GetId() const {
  return static_cast<int>(screen_.lock()->GetId());
}

void Screen::Clear() {
  screen_.lock()->Clear();
}

void Screen::SelectColorPair(uint8_t pair_id) {
  screen_.lock()->SelectColorPair(pair_id);
}

void Screen::MoveTo(int x, int y) {
  screen_.lock()->MoveTo(x, y);
}

void Screen::PrintW(const std::string& str) {
  screen_.lock()->PrintW(str);
}

void Screen::Refresh() {
  screen_.lock()->Refresh();
}

}  // namespace py

}  // namespace m2d
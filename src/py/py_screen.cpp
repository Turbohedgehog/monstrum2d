#include "py/py_screen.h"

#include "terminal/screen.h"

#include "curses.h"

namespace m2d {

namespace py {

#if 0
#define COLOR_BLACK   0

#ifdef PDC_RGB        /* RGB */
# define COLOR_RED    1
# define COLOR_GREEN  2
# define COLOR_BLUE   4
#else                 /* BGR */
# define COLOR_BLUE   1
# define COLOR_GREEN  2
# define COLOR_RED    4
#endif

#define COLOR_CYAN    (COLOR_BLUE | COLOR_GREEN)
#define COLOR_MAGENTA (COLOR_RED | COLOR_BLUE)
#define COLOR_YELLOW  (COLOR_RED | COLOR_GREEN)

#define COLOR_WHITE   7

#endif

//enum class Color1 {
enum Color {
  Black = COLOR_BLACK,
  While = COLOR_WHITE,
  Red = COLOR_RED,
  Green = COLOR_GREEN,
  Blue = COLOR_BLUE,
  Cyan = COLOR_CYAN,
  Magenta = COLOR_MAGENTA,
  Yellow = COLOR_YELLOW,
};

bp::object Screen::CreateColorEnumDeclaration() {
  return bp::enum_<Color>("Color")
      .value("Black", Black)
      .value("While", While)
      .value("Red", Red)
      .value("Green", Green)
      .value("Blue", Blue)
      .value("Cyan", Cyan)
      .value("Magenta", Magenta)
      .value("Yellow", Yellow)
      //.export_values()
      ;
}

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

  auto p = static_cast<short>(pair_id);
  auto f = static_cast<short>(foreground);
  auto b = static_cast<short>(background);

  screen_.lock()->SetColorPair(p, f, b);
}

void Screen::SetClearColorPair(int pair_id) {
  if (pair_id < 0 || pair_id > 255) {
    return;
  }

  auto p = static_cast<short>(pair_id);

  screen_.lock()->SetClearColorPair(p);
}

int Screen::GetId() const {
  return static_cast<int>(screen_.lock()->GetId());
}

void Screen::Clear() {
  screen_.lock()->Clear();
}

void Screen::SelectColorPair(short pair_id) {
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
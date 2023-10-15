#pragma once

#include "py/py.h"

#include "terminal/types.h"

namespace m2d {

namespace py {

class Screen {
 public:
  static bp::object CreateClassDeclaration();
  static bp::object CreateColorEnumDeclaration();

  void SetScreen(hi::ScreenWeakPtr screen);
  bp::object GetKeyPressed() const;

  void SetColorPair(int pair_id, int foreground, int background);
  void SetClearColorPair(int pair_id);
  int GetId() const;
  void Clear();

  void SelectColorPair(short pair_id);
  void MoveTo(int x, int y);
  void PrintW(const std::string& str);
  void Refresh();

 private:
  hi::ScreenWeakPtr screen_;
};

}  // namespace py

}  // namespace m2d

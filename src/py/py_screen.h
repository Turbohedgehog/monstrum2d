#pragma once

#include "py/py.h"

#include "terminal/types.h"

namespace m2d {

namespace py {

class Screen {
 public:
  static bp::object CreateClassDeclaration();

  void SetScreen(hi::ScreenWeakPtr screen);
  bp::object GetKeyPressed() const;

  void SetColorPair(int pair_id, int foreground, int background);
  void SetClearColorPair(int pair_id);
  int GetId() const;
  void Clear();

 private:
  hi::ScreenWeakPtr screen_;
};

}  // namespace py

}  // namespace m2d

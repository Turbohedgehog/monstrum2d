#pragma once

#include "terminal/types.h"

namespace m2d {

namespace hi {

class Screen {
 public:
  Screen(std::size_t id);

  void Update(float delta);

  void SetKeyPressed(KeyCode key_pressed);
  KeyCode GetKeyPressed() const;

 private:
  std::size_t id_ = 0;
  KeyCode key_pressed_;
};

}  // namespace hi

}  // namespace m2d

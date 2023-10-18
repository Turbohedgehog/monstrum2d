#pragma once

#include <iostream>
#include <map>

#include "terminal/types.h"

namespace m2d {

namespace hi {

class Terminal {
 public:
  static Terminal& GetInstance();

  ScreenWeakPtr AppendScreen();
  void Update(float delta);
  void Sleep(int milliseconds);
  ScreenWeakPtr GetScreen(std::size_t id) const;
  void Shutdown();
  ScreenWeakPtr SetActiveScreen(std::size_t id);
  void SetSize(int width, int height);
  int GetWidth() const;
  int GetHeight() const;

 private:
  Terminal();

  std::size_t active_screen_counter_ = 0;
  std::map<std::size_t, ScreenPtr> active_screens_;
  ScreenPtr active_screen_;
  //std::ostream ostream_;
  int height_ = 0;
  int width_ = 0;
};

}  // namespace hi

}  // namespace m2d

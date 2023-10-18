#pragma once

#include <array>
#include <tuple>
#include <string>

#include "terminal/types.h"

namespace m2d {

namespace hi {

class Screen {
 public:
  Screen(std::size_t id);
  std::size_t GetId() const;


  void Update(float delta);

  void OnActivate();
  void OnDeactivate();
  void SetColorPair(short pair_id, short foreground, short background);
  void SetClearColorPair(short pair_id);
  
  void SetKeyPressed(KeyCode key_pressed);
  KeyCode GetKeyPressed() const;
  void Clear();

  void SelectColorPair(short pair_id);
  void MoveTo(int x, int y);
  void PrintW(const std::string& str);
  void Refresh();

 private:
  std::size_t id_ = 0;
  KeyCode key_pressed_;
  bool is_active_ = false;

  std::array<std::tuple<short, short>, 8> color_scheme_;
  short clear_screen_pair_ = 0;
};

}  // namespace hi

}  // namespace m2d

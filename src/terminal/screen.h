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
  void SetColorPair(uint8_t pair_id, uint8_t foreground, uint8_t background);
  void SetClearColorPair(uint8_t pair_id);
  
  void SetKeyPressed(KeyCode key_pressed);
  KeyCode GetKeyPressed() const;
  void Clear();

  void SelectColorPair(uint8_t pair_id);
  void MoveTo(int x, int y);
  void PrintW(const std::string& str);

 private:
  std::size_t id_ = 0;
  KeyCode key_pressed_;
  bool is_active_ = false;

  std::array<std::tuple<uint8_t, uint8_t>, 8> color_scheme_;
  uint8_t clear_screen_pair_ = 0;
};

}  // namespace hi

}  // namespace m2d

#include "terminal/screen.h"

#include <iostream>

#include "curses.h"

namespace m2d {

namespace hi {

Screen::Screen(std::size_t id, _win* window)
    : id_(id)
    , window_(window) {
  for (auto i = 0 ; i < color_scheme_.size(); ++i) {
    color_scheme_[i] = std::make_tuple<uint8_t, uint8_t>(COLOR_WHITE, COLOR_BLACK);
  }
}

std::size_t Screen::GetId() const {
  return id_;
}

void Screen::SetKeyPressed(KeyCode key_pressed) {
  key_pressed_ = key_pressed;
}

void Screen::Update(float delta) {
  #if 0
  //clear();
  bkgd(COLOR_PAIR(1));

  //int key = -1;
  //std::cout << "key = " << key << "\n";
  move(10, 10);
  attrset(A_DIM | COLOR_PAIR(5));
  printw("WWWWWWWWWW");
  #endif
  
  int key = getch();
  if (key > 0) { 
    //printw("   %d", key);
    SetKeyPressed(key);
  } else {
    SetKeyPressed(std::nullopt);
  }
}

void Screen::SelectColorPair(short pair_id) {
  attrset(A_DIM | COLOR_PAIR(pair_id));
}

void Screen::MoveTo(int x, int y) {
  //wmove(window_, y, x);
  move(y, x);
}

void Screen::PrintW(const std::string& str) {
  //wprintw(window_, str.c_str());
  printw(str.c_str());
}

KeyCode Screen::GetKeyPressed() const {
  return key_pressed_;
}

void Screen::OnActivate() {
  is_active_ = true;

  for (auto i = 0 ; i < color_scheme_.size(); ++i) {
    const auto& colors = color_scheme_[i];
    init_pair(i, std::get<0>(colors), std::get<1>(colors));
  }

  //wbkgd(window_, clear_screen_pair_);
  bkgd(COLOR_PAIR(clear_screen_pair_));

  //init_pair(1, COLOR_WHITE, COLOR_BLUE);
  //bkgd(COLOR_PAIR(1));
}

void Screen::OnDeactivate() {
  is_active_ = false;
}

void Screen::SetColorPair(short pair_id, short foreground, short background) {
  color_scheme_[pair_id] = std::make_tuple(foreground, background);
  if (is_active_) {
    init_pair(pair_id, foreground, background);
  }
}

void Screen::SetClearColorPair(short pair_id) {
  clear_screen_pair_ = pair_id;

  bkgd(COLOR_PAIR(clear_screen_pair_));
  //Clear();
}

void Screen::Clear() {
  //bkgd(COLOR_PAIR(clear_screen_pair_));
  clear();
  //wclear(window_);
}

void Screen::Refresh() {
  //wrefresh(window_);
}

}  // namespace hi

}  // namespace m2d

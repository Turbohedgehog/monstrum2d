#include "terminal/screen.h"

#include <iostream>

#include "curses.h"

namespace m2d {

namespace hi {

Screen::Screen(std::size_t id)
    : id_(id) {}

void Screen::SetKeyPressed(KeyCode key_pressed) {
  /*
  if (key_pressed) {
    std::cout << "SetKeyPressed " << key_pressed.value() << "\n";
  }
  */
  key_pressed_ = key_pressed;
}

void Screen::Update(float delta) {
  //clear();

  int key = getch();
  //int key = -1;
  //std::cout << "key = " << key << "\n";
  move(10, 10);
  //attrset(A_DIM | COLOR_PAIR(5));
  printw("WWWWWWWWWW");
  if (key > 0) { 
    printw("   %d", &key);
    SetKeyPressed(key);
    if (key == 27) {
      //exit(0);
    }
  }
}

KeyCode Screen::GetKeyPressed() const {
  /*
  if (key_pressed_) {
    std::cout << "GetKeyPressed " << key_pressed_.value() << "\n";
  }
  if (key_pressed_) {
    std::cout << "GetKeyPressed " << key_pressed_.value() << "\n";
  } else {
    std::cout << "NoGetKeyPressed\n";
  }
  */
  return key_pressed_;
}

}  // namespace hi

}  // namespace m2d

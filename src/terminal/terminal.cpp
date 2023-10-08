#include "terminal/terminal.h"

#include "curses.h"

#include "terminal/screen.h"

namespace m2d {

namespace hi {

Terminal::Terminal() {
  //AppendScreen();
  
  auto screen = initscr();
  keypad(screen, true);
  //noecho();
  curs_set(0);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_MAGENTA, COLOR_BLUE);
  init_pair(3, COLOR_GREEN, COLOR_BLUE);
  init_pair(4, COLOR_RED, COLOR_BLUE);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_WHITE, COLOR_RED);

  bkgd(COLOR_PAIR(6));
}

Terminal& Terminal::GetInstance() {
  static Terminal instance;

  return instance;
}

ScreenWeakPtr Terminal::AppendScreen() {
  auto screen = std::make_shared<Screen>(active_screen_counter_);
  active_screens_[active_screen_counter_] = screen;
  if (!active_screen_) {
    active_screen_ = screen;
  }

  ++active_screen_counter_;

  return screen;
}

void Terminal::Sleep(int milliseconds) {
  //timeout(milliseconds);
}

void Terminal::Shutdown() {
  //endwin();
}

void Terminal::Update(float delta) {
  if (active_screen_) {
    active_screen_->Update(delta);
  }
}

}  // namespace hi

}  // namespace m2d
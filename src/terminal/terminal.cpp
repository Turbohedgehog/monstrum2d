#include "terminal/terminal.h"

#include "curses.h"

#include "terminal/screen.h"

namespace m2d {

namespace hi {

Terminal::Terminal() {}

Terminal& Terminal::GetInstance() {
  static Terminal instance;

  return instance;
}

ScreenWeakPtr Terminal::AppendScreen() {
  if (active_screens_.empty()) {
    auto screen = initscr();
    keypad(screen, true);
    start_color();
    noecho();
    curs_set(0);
    raw();
  }

  auto screen = std::make_shared<Screen>(active_screen_counter_);
  active_screens_[active_screen_counter_] = screen;
  if (!active_screen_) {
    active_screen_ = screen;
    screen->OnActivate();
  }

  ++active_screen_counter_;

  return screen;
}

void Terminal::Sleep(int milliseconds) {
  timeout(milliseconds);
}

void Terminal::Shutdown() {
}

ScreenWeakPtr Terminal::GetScreen(std::size_t id) const {
  auto it = active_screens_.find(id);

  return it != active_screens_.end() ? it->second : ScreenWeakPtr();
}

void Terminal::Update(float delta) {
  if (active_screen_) {
    active_screen_->Update(delta);
  }
}

ScreenWeakPtr Terminal::SetActiveScreen(std::size_t id) {
  auto screen = GetScreen(id);
  if (!screen.expired()) {
    if (active_screen_) {
      active_screen_->OnDeactivate();
    }

    active_screen_ = screen.lock();
    active_screen_->OnActivate();
  }

  return screen;
}

void Terminal::SetSize(int width, int height) {
  width_ = width;
  height_ = height;
}

int Terminal::GetWidth() const {
  return width_;
}

int Terminal::GetHeight() const {
  return height_;
}

}  // namespace hi

}  // namespace m2d
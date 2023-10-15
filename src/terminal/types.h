#pragma once

#include <memory>

#include <optional>

struct _win;

namespace m2d {

namespace hi {

class Screen;
using ScreenPtr = std::shared_ptr<Screen>;
using ScreenWeakPtr = std::weak_ptr<Screen>;

class Widget;
using WidgetPtr = std::shared_ptr<Widget>;

using KeyCode = std::optional<int>;

}  // namespace hi

}  // namespace m2d

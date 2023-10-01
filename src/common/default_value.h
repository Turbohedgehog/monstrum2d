#pragma once

#include <string>
#include <string_view>

namespace m2d {

template <typename T>
struct DefaultValue {
  static constexpr T value = T(0);
};

template <>
struct DefaultValue<std::string> {
  inline static const std::string value = "";
};

}  // namespace m2d
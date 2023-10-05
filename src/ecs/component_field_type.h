#pragma once

#include <string>

namespace m2d {

namespace ecs {

enum class ComponentFiledType {
  Int,
  Double,
  String,
  Array,
  Struct,
};

template <typename T>
struct ComponentType {};

template <>
struct ComponentType<int> {
  static constexpr ComponentFiledType value = ComponentFiledType::Int;
};

template <>
struct ComponentType<double> {
  static constexpr ComponentFiledType value = ComponentFiledType::Double;
};

template <>
struct ComponentType<std::string> {
  static constexpr ComponentFiledType value = ComponentFiledType::String;
};

template <typename T>
static constexpr ComponentFiledType ComponentTypeV = ComponentType<T>::value;

}  // namespace ecs

}  // namespace m2d

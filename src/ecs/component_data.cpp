#include "ecs/component_data.h"

namespace m2d {

namespace ecs {

ComponentData::ComponentData(std::size_t size) {
  data_.reserve(size);
}

ComponentData::ComponentData(const ComponentData& component_data)
  : data_(component_data.data_) {
}

std::vector<char>& ComponentData::GetData() {
  return data_;
}

const std::vector<char>& ComponentData::GetData() const {
  return data_;
}

}  // namespace ecs

}  // namespace m2d

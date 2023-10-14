#include "ecs/component_data.h"

#include <algorithm>

//#include "ecs.h"
#include "ecs/component_schema.h"

namespace m2d {

namespace ecs {

void ArrayComponentData::Resize(std::size_t size) {
  if (size == data.size()) {
    return;
  }

  if (size > data.size()) {
    Insert(data.size(), size - data.size());
  } else {
    auto diff = size - data.size();
    Remove(data.size(), data.size() - size);
  }
}

void ArrayComponentData::Insert(std::size_t idx, std::size_t count) {
  data.insert(std::next(data.begin() + idx), count, ComponentDataPtr());

  for (std::size_t i = 0; i < count; ++i) {
    data[idx + i] = content_field->AllocateData(ecs);
  }
}

void ArrayComponentData::Remove(std::size_t idx, std::size_t count) {
  data.erase(std::next(data.begin() + idx), std::next(data.begin() + idx + count - 1));
}

}  // namespace ecs

}  // namespace m2d
#pragma once

#include <vector>
#include <string>

#include "common/common_types.h"

namespace m2d {

namespace ecs {

struct ComponentDataBase {};

template <typename T>
struct ComponentPrimitiveData : ComponentDataBase {
  T data;
};

using IntComponentData = ComponentPrimitiveData<int>;
using DoubleComponentData = ComponentPrimitiveData<double>;
using StringComponentData = ComponentPrimitiveData<std::string>;

struct StructComponentData  : ComponentDataBase {
  std::vector<ComponentDataPtr> datalist;
};

}  // namespace ecs

}  // namespace m2d

#pragma once

#include <vector>
#include <string>

#include "common/common_types.h"

#include "ecs/component_field_type.h"

namespace m2d {

namespace ecs {

struct ComponentDataBase {
  virtual ComponentFiledType GetType() const = 0;
};

template <typename T>
struct ComponentPrimitiveData : ComponentDataBase {
  ComponentFiledType GetType() const override {
    return ComponentTypeV<T>;
  }

  T data;
};

using IntComponentData = ComponentPrimitiveData<int>;
using DoubleComponentData = ComponentPrimitiveData<double>;
using StringComponentData = ComponentPrimitiveData<std::string>;

struct ArrayComponentData : ComponentDataBase {
  std::vector<ComponentDataPtr> data;
  ComponentFieldPtr content_field;
  ECSWeakPtr ecs;
  ComponentFiledType GetType() const override {
    return ComponentFiledType::Array;
  }

  void Resize(std::size_t size);
  void Insert(std::size_t idx, std::size_t count = 1);
  void Remove(std::size_t idx, std::size_t count = 1);
};

struct StructComponentData : ComponentDataBase {
  std::vector<ComponentDataPtr> data;

  ComponentFiledType GetType() const override {
    return ComponentFiledType::Struct;
  }
};

}  // namespace ecs

}  // namespace m2d

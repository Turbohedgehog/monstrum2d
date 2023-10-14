#pragma once

#include "py/py.h"

#include "common/common_types.h"

#include "py/py_component.h"

namespace m2d {

namespace py {

class Array {
 public:
  static bp::object CreateClassDeclaration();

  void SetData(ecs::ArrayComponentDataWeakPtr array_data);
  
  int Len() const;
  void Resize(int size);
  void Insert(int idx, int count = 1);
  void Remove(int idx, int count = 1);

 private:
  ecs::ArrayComponentDataWeakPtr array_data_;
};

}  // namespace py

}  // namespace m2d

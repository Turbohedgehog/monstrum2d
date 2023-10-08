#include "py/py_array.h"

#include "ecs/component_data.h"

namespace m2d {

namespace py {

bp::object Array::CreateClassDeclaration() {
  return bp::class_<Array>("ComponentSchema")
      .def("resize", &Array::Resize, (bp::arg("size")))
      .def("insert", &Array::Insert, (bp::arg("index"), bp::arg("count") = 1))
      .def("remove", &Array::Remove, (bp::arg("index"), bp::arg("count") = 1))
      .def("__len__", &Array::Len)
  ;
}

void Array::SetData(ecs::ArrayComponentDataWeakPtr array_data) {
  array_data_ = array_data;
}

int Array::Len() const {
  return static_cast<int>(array_data_.lock()->data.size());
}

void Array::Resize(int size) {
  if (size < 0) {
    return;
  }

  array_data_.lock()->Resize(static_cast<std::size_t>(size));
}

void Array::Insert(int idx, int count) {
  if (idx < 0 || count < 0) {
    return;
  }

  auto v_idx = static_cast<std::size_t>(idx);
  auto v_count = static_cast<std::size_t>(count);
  array_data_.lock()->Insert(v_idx, v_count);
}

void Array::Remove(int idx, int count) {
  if (idx < 0 || count < 0) {
    return;
  }

  auto v_idx = static_cast<std::size_t>(idx);
  auto v_count = static_cast<std::size_t>(count);
  array_data_.lock()->Remove(v_idx, v_count);
}

}  // namespace py

}  // namespace m2d
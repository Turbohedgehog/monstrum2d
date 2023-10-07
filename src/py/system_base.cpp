#include "py/system_base.h"

namespace m2d {

namespace py {

SystemBase::SystemBase(bp::object system_handler)
    : system_handler_(system_handler) {}

void SystemBase::Update(float delta) {}

bp::object SystemBase::GetSystemHandler() const {
  return system_handler_;
}

bp::object SystemBase::CreateClassDeclaration() {
  return bp::class_<m2d::py::SystemBase>("SystemBase", bp::init<bp::object>(bp::args("system_handler")))
      .def("update", &m2d::py::SystemBase::Update, bp::args("delta"))
      .add_property("system_handler", &m2d::py::SystemBase::GetSystemHandler)
  ;
}

}  // namespace py

}  // namespace m2d

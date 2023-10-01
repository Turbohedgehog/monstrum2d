#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <string>

#include <boost/python.hpp>

namespace bp = boost::python;

namespace m2d {

namespace py {

class SystemBase {
 public:
  SystemBase(bp::object system_handler)
      : system_handler_(system_handler) {}

  void Update(float delta) {
    volatile int zzz = 0;
  }


  bp::object GetSystemHandler() const {
    return system_handler_;
  }

 private:
  bp::object system_handler_;
};

}  // namespace py

}  // namespace m2d

BOOST_PYTHON_MODULE(Core) {
  bp::class_<m2d::py::SystemBase>("SystemBase", bp::init<bp::object>())
  //bp::class_<m2d::py::SystemBase>("SystemBase")
      .def("update", &m2d::py::SystemBase::Update, bp::args("delta"))
      //.def("register_system_handler", &m2d::py::SystemBase::RegisterSyetemHandler, bp::args("system_handler"))
      .add_property("system_handler", &m2d::py::SystemBase::GetSystemHandler);
  ;
      //.def("update", bp::pure_virtual(&m2d::py::SystemBase::Update), bp::args("delta"))
      //.add_property("enable_update", &m2d::py::SystemBase::GetEnableUpdate, &m2d::py::SystemBase::SetEnableUpdate);
}
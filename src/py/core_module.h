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
      : system_handler_(system_handler) {
    volatile int zzz = 0;
  }

  bool GetEnableUpdate() const {
    return enable_update_;
  }

  void SetEnableUpdate(bool enable_update) {
    enable_update_ = enable_update;
    system_handler_.attr("update_property_changed")(bp::ptr(this));
  }

 private:
  bp::object system_handler_;
  bool enable_update_ = false;
};

}  // namespace py

}  // namespace m2d

BOOST_PYTHON_MODULE(Core) {
  bp::class_<m2d::py::SystemBase>("SystemBase", bp::init<bp::object>())
      .add_property("update", &m2d::py::SystemBase::GetEnableUpdate, &m2d::py::SystemBase::SetEnableUpdate);
}
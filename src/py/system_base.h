#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

namespace bp = boost::python;

namespace m2d {

namespace py {

class SystemBase {
 public:
  SystemBase(bp::object system_handler);

  void Update(float delta);

  bp::object GetSystemHandler() const;

  static bp::object CreateClassDeclaration();

 private:
  bp::object system_handler_;
};

}  // namespace py

}  // namespace m2d
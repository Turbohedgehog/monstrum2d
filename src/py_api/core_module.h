#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

namespace m2d {

namespace py {

class SystemBase {

};

namespace bp = boost::python;

BOOST_PYTHON_MODULE(Core) {
  bp::class_<SystemBase, boost::noncopyable>("SystemBase", bp::no_init);
}

}  // namespace py

}  // namespace m2d

#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <filesystem>

#include <boost/python.hpp>

namespace m2d {

namespace py {

class Application {
 public:
  Application();
  ~Application();

  void CollectSystems(const std::filesystem::path& system_path);
  
 private:
  static std::size_t instance_count_;

  boost::python::object global_;
  boost::python::object builtins_module_;
  boost::python::object inspect_module_;
  boost::python::object system_base_class_;
};

}  // namespace py

}  // namespace m2d

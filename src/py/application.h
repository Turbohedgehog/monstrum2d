#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <filesystem>
#include <memory>

#include <boost/python.hpp>

namespace m2d {

namespace py {

class SystemHandler;

class Application {
 public:
  Application();
  ~Application();

  void CollectSystems(const std::filesystem::path& system_path);

  void InitSystems();
  void Update(float delta);
  bool IsActive() const;
  
 private:
  static std::size_t instance_count_;

  boost::python::object global_;
  boost::python::object builtins_module_;
  boost::python::object inspect_module_;
  boost::python::object system_base_class_;

  std::shared_ptr<SystemHandler> system_handler_;
};

}  // namespace py

}  // namespace m2d

#pragma once

#include <string>
#include <map>

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>


namespace bp = boost::python;

namespace m2d {

namespace py {

struct SystemHolder {
  std::string name;
  bp::object source_class;
  bp::object instance;
};

class SystemHandler {
 public:
  SystemHandler();
  void RegisterSystem(const std::string& system_name, bp::object system_class);
  void InstantiateSystems();
  void Update(float delta);
  bool IsActive() const;

 private:
  void RegisterHandlerClass();
  void OnUpdatePropertyChanged(bp::object system_object);

  std::map<std::string, SystemHolder> systems_;

  boost::python::object handler_class_;
};

}  // namespace py

}  // namespace m2d

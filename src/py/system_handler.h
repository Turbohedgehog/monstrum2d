#pragma once

#include <string>
#include <map>
#include <set>
#include <unordered_map>

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"


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
  ~SystemHandler();
  void RegisterSystem(const std::string& system_name, bp::object system_class);
  void InstantiateSystems(ecs::HolderWeakPtr ecs_holder);
  void Update(float delta);
  bool IsActive() const;
  //void OnEnableUpdatePropertyChanged(bp::object system_object);
  void RegisterHandlerClass();
  void EnableSystemUpdate(bp::object system_object);
  void DisableSystemUpdate(bp::object system_object);
  void ShutdownHolder();

 private:
  ecs::HolderWeakPtr ecs_holder_;
  std::map<std::string, SystemHolder> systems_;
  std::map<PyObject*, bp::object> systems_to_update_;
  //std::map<bp::object, bp::object> systems_to_update_;
  //std::unordered_map<bp::object, bp::object> systems_to_update_;
};

}  // namespace py

}  // namespace m2d

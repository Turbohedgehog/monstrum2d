#pragma once

#include <string>
#include <map>
#include <set>
#include <unordered_map>

#include "py/py.h"

#include "common/common_types.h"

#include "py/py_ecs.h"
#include "py/py_holder.h"


namespace m2d {

namespace py {

struct SystemHolder {
  std::string name;
  bp::object source_class;
  bp::object instance;
};

class SystemHandler {
 public:
  void RegisterSystem(const std::string& system_name, bp::object system_class);
  void InstantiateSystems(ecs::HolderWeakPtr ecs_holder);
  void Update(float delta);
  bool IsActive() const;
  void RegisterHandlerClass();
  void EnableSystemUpdate(bp::object system_object);
  void DisableSystemUpdate(bp::object system_object);
  Holder GetHolder() const;
  void SetBuiltinsModule(bp::object builtins_module);

 private:
  ecs::HolderWeakPtr ecs_holder_;
  std::map<std::string, SystemHolder> systems_;
  std::map<std::string, bp::object> systems_to_update_;
};

}  // namespace py

}  // namespace m2d

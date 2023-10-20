#include "py/system_handler.h"

#include <stdexcept>
#include <iostream>

#include <boost/format.hpp>

namespace m2d {

namespace py {

SystemHandler::SystemHandler() {
  //RegisterHandlerClass();
}

SystemHandler::~SystemHandler() {
  volatile int zzz = 0;
}

void SystemHandler::Update(float delta) {
  for (auto& [object, update_func] : systems_to_update_) {
    //std::string class_name = bp::extract<std::string>(bp::object(object).attr("__class__").attr("__name__"));
    update_func(delta);
  }
}

void SystemHandler::RegisterSystem(const std::string& system_name, bp::object system_class) {
  if (systems_.find(system_name) != systems_.end()) {
    throw std::runtime_error(
      (
        boost::format("System '%s' already registered!") %
          system_name
      ).str()
    );
  }

  systems_[system_name] = SystemHolder {
    .name = system_name,
    .source_class = system_class,
  };
}

void SystemHandler::InstantiateSystems(ecs::HolderWeakPtr ecs_holder) {
  ecs_holder_ = ecs_holder;
  for (auto& [_, holder] : systems_) {
    //holder.instance = holder.source_class(this);
    holder.instance = holder.source_class(bp::ptr(this));
    //holder.instance = holder.source_class();
    //std::string class_name = bp::extract<std::string>(holder.instance.attr("__class__").attr("__name__"));
    //holder.instance.attr("register_system_handler")(bp::ptr(this));
    //holder.instance.attr("update")(0.1);
  }
}

bool SystemHandler::IsActive() const {
  return !systems_to_update_.empty();
}

/*
void SystemHandler::OnEnableUpdatePropertyChanged(bp::object system_object) {
  std::string class_name = bp::extract<std::string>(system_object.attr("__class__").attr("__name__"));
  bool enable_update = bp::extract<bool>(system_object.attr("enable_update"));
  if (enable_update) {
    systems_to_update_.insert({system_object, system_object.attr("update")});
  } else {
    auto it = systems_to_update_.find(system_object);
    if (it != systems_to_update_.end()) {
      systems_to_update_.erase(it);
    }
  }
}
*/

void SystemHandler::EnableSystemUpdate(bp::object system_object) {
  std::string class_name = bp::extract<std::string>(system_object.attr("__class__").attr("__name__"));
  //systems_to_update_.insert({system_object.ptr(), system_object.attr("update")});
  systems_to_update_.insert({class_name, system_object.attr("update")});
}

void SystemHandler::DisableSystemUpdate(bp::object system_object) {
  std::string class_name = bp::extract<std::string>(system_object.attr("__class__").attr("__name__"));
  //auto it = systems_to_update_.find(system_object.ptr());
  auto it = systems_to_update_.find(class_name);
  if (it != systems_to_update_.end()) {
    systems_to_update_.erase(it);
  }
}

void SystemHandler::RegisterHandlerClass() {
  bp::class_<SystemHandler>("SystemHandler")
      .def("enable_system_update", &SystemHandler::EnableSystemUpdate, bp::args(("system_object")))
      .def("disable_system_update", &SystemHandler::DisableSystemUpdate, bp::args(("system_object")))
      //.def("shutdown", &SystemHandler::ShutdownHolder)
      .def("get_holder", &SystemHandler::GetHolder)
  ;
}

Holder SystemHandler::GetHolder() const {
  Holder holder;
  holder.SetHolderPtr(ecs_holder_);

  return holder;
}

}  // namespace py

}  // namespace m2d

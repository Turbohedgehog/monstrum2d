#include "py/system_handler.h"

#include <stdexcept>
#include <iostream>

#include <boost/format.hpp>

namespace m2d {

namespace py {

void SystemHandler::Update(float delta) {
  for (auto& [object, update_func] : systems_to_update_) {
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
    holder.instance = holder.source_class(bp::ptr(this));
    EnableSystemUpdate(holder.instance);
  }
}

bool SystemHandler::IsActive() const {
  return !systems_to_update_.empty();
}

void SystemHandler::EnableSystemUpdate(bp::object system_object) {
  std::string class_name = bp::extract<std::string>(system_object.attr("__class__").attr("__name__"));
  systems_to_update_.insert({class_name, system_object.attr("update")});
}

void SystemHandler::DisableSystemUpdate(bp::object system_object) {
  std::string class_name = bp::extract<std::string>(system_object.attr("__class__").attr("__name__"));
  auto it = systems_to_update_.find(class_name);
  if (it != systems_to_update_.end()) {
    systems_to_update_.erase(it);
  }
}

void SystemHandler::RegisterHandlerClass() {
  bp::class_<SystemHandler>("SystemHandler")
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

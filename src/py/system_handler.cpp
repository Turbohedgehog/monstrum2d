#include "py/system_handler.h"

#include <stdexcept>
#include <iostream>

#include <boost/format.hpp>

namespace m2d {

namespace py {

SystemHandler::SystemHandler() {
  RegisterHandlerClass();
}

void SystemHandler::Update(float delta) {

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

void SystemHandler::InstantiateSystems() {
  for (auto& [_, holder] : systems_) {
    holder.instance = holder.source_class(this);
  }
}

bool SystemHandler::IsActive() const {
  return false;
}

void SystemHandler::OnUpdatePropertyChanged(bp::object system_object) {
  bool b = bp::extract<bool>(system_object.attr("update"));
  std::cout << b;
}

void SystemHandler::RegisterHandlerClass() {
  bp::class_<SystemHandler>("SystemHandler")
      .def("update_property_changed", &SystemHandler::OnUpdatePropertyChanged);
}

}  // namespace py

}  // namespace m2d

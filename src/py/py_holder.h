#pragma once

#include <map>
#include <string>

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "common/common_types.h"

#include "py/py_ecs.h"

#include "py/py_component_schema.h"

namespace bp = boost::python;

namespace m2d {

namespace py {

class Holder {
 public:
  static bp::object CreateClassDeclaration();

  void SetHolderPtr(ecs::HolderWeakPtr holder);
  void Shutdown();
  ECS GetOrCreateECS(const std::string& ecs_name);

  //boost::optional<ComponentSchema> GetComponentSchema(const std::string& schema_name);
  //boost::optional<int> GetComponentSchemaId(const std::string& schema_name);

  bp::object GetComponentSchema(const std::string& schema_name);
  bp::object GetComponentSchemaId(const std::string& schema_name);

 private:
  ecs::HolderWeakPtr holder_;
  std::map<std::string, ComponentSchema> component_schemas_;

};

}  // namespace py

}  // namespace m2d
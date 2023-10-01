#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "py/system_base.h"
#include "py/py_ecs.h"
#include "py/py_holder.h"
#include "py/py_entity.h"
#include "py/py_filter.h"
#include "py/py_component.h"
#include "py/py_component_schema.h"

BOOST_PYTHON_MODULE(Core) {
  m2d::py::SystemBase::CreateClassDeclaration();
  m2d::py::ECS::CreateClassDeclaration();
  m2d::py::Entity::CreateClassDeclaration();
  m2d::py::Filter::CreateClassDeclaration();
  m2d::py::Component::CreateClassDeclaration();
  m2d::py::Holder::CreateClassDeclaration();
  m2d::py::ComponentSchema::GetClassDeclaration();
}
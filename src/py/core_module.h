#pragma once

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "py/system_base.h"
#include "py/ecs_wrapper.h"

BOOST_PYTHON_MODULE(Core) {
  m2d::py::SystemBase::CreateClassDeclaration();
  m2d::py::ECS::CreateClassDeclaration();
}
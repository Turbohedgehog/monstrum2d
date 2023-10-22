#pragma once

#include "py/py.h"

#include "py/system_base.h"
#include "py/py_ecs.h"
#include "py/py_holder.h"
#include "py/py_entity.h"
#include "py/py_filter.h"
#include "py/py_component.h"
#include "py/py_terminal.h"
#include "py/py_screen.h"
#include "py/py_array.h"
#include "py/py_vector2d.h"

BOOST_PYTHON_MODULE(Core) {
  //PyEval_InitThreads();
  //bp::scope().attr("__path__") = "Core";

  m2d::py::Screen::CreateColorEnumDeclaration();  
  m2d::py::Terminal::CreateClassDeclaration();
  m2d::py::SystemBase::CreateClassDeclaration();
  m2d::py::ECS::CreateClassDeclaration();
  m2d::py::Entity::CreateClassDeclaration();
  m2d::py::Filter::CreateClassDeclaration();
  m2d::py::Component::CreateClassDeclaration();
  m2d::py::Holder::CreateClassDeclaration();
  m2d::py::Screen::CreateClassDeclaration();
  m2d::py::Array::CreateClassDeclaration();
  m2d::py::IntVector2D::CreateClassDeclaration();
}

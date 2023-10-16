#include "py/py_vector2d.h"

#include <boost/format.hpp>

namespace m2d {

namespace py {

// Stolen from https://github.com/carla-simulator/carla/blob/master/PythonAPI/carla/source/libcarla/Geom.cpp#L146
bp::object IntVector2D::CreateClassDeclaration() {
  //return bp::class_<IntVector2D, bp::bases<Vector2D<int>>>("IntVector2D")
  return bp::class_<IntVector2D>("IntVector2D")
      .def(bp::init<int, int>((bp::arg("x")=0, bp::arg("y")=0)))
      .def(bp::init<IntVector2D>((bp::arg("v"))))
      .def_readwrite("x", &IntVector2D::x_)
      .def_readwrite("y", &IntVector2D::y_)
      .def("squared_length", &IntVector2D::SquaredLength)
      .def("length", &IntVector2D::Length)
      .def("make_unit_vector", &IntVector2D::MakeUnitVector)
      .def("__eq__", &IntVector2D::operator==)
      .def("__ne__", &IntVector2D::operator!=)
      .def(bp::self += bp::self)
      .def(bp::self + bp::self)
      .def(bp::self -= bp::self)
      .def(bp::self - bp::self)
      .def(bp::self *= int())
      .def(bp::self * int())
      .def(int() * bp::self)
      .def(bp::self /= int())
      .def(bp::self / int())
      .def(bp::self / bp::self)
      .def(int() / bp::self)
      //.def(bp::str(bp::self))
      .def(bp::self_ns::str(bp::self_ns::self))
      //.def(bp::self_ns::str(bp::self))
  ;
}

bp::object DoubleVector2D::CreateClassDeclaration() {
  return bp::class_<DoubleVector2D, bp::bases<Vector2D<double>>>("DoubleVector2D")
      .def(bp::init<double, double>((bp::arg("x")=0.0, bp::arg("y")=0.0)))
      .def(bp::init<DoubleVector2D>((bp::arg("v"))))
      .def_readwrite("x", &DoubleVector2D::x_)
      .def_readwrite("y", &DoubleVector2D::y_)
      .def("squared_length", &DoubleVector2D::SquaredLength)
      .def("length", &DoubleVector2D::Length)
      .def("make_unit_vector", &DoubleVector2D::MakeUnitVector)
      .def("__eq__", &DoubleVector2D::operator==)
      .def("__ne__", &DoubleVector2D::operator!=)
      .def(bp::self += bp::self)
      .def(bp::self + bp::self)
      .def(bp::self -= bp::self)
      .def(bp::self - bp::self)
      .def(bp::self *= double())
      .def(bp::self * double())
      .def(double() * bp::self)
      .def(bp::self /= double())
      .def(bp::self / double())
      .def(bp::self / bp::self)
      .def(double() / bp::self)
      //.def(bp::self_ns::str(bp::self_ns::self))
  ;
}

#if 1
IntVector2D::IntVector2D(int x, int y)
    : x_(x)
    , y_(y) {}

IntVector2D::IntVector2D(const IntVector2D& other)
    : IntVector2D(other.x_, other.y_) {}  

int IntVector2D::SquaredLength() const {
  return x_ * x_ + y_ * y_;
}

int IntVector2D::Length() const {
  return std::sqrt(SquaredLength());
}

IntVector2D IntVector2D::MakeUnitVector() const {
  return *this / Length();
}

int IntVector2D::X() const {
  return x_;
}

int IntVector2D::Y() const {
  return y_;
}

bool IntVector2D::operator==(const IntVector2D& other) const {
  if (this == &other) {
    return true;
  }

  return x_ == other.x_ && y_ == other.y_;
}

bool IntVector2D::operator!=(const IntVector2D& other) const {
  return !operator==(other);
}

IntVector2D& IntVector2D::operator += (const IntVector2D& other) {
  x_ += other.x_;
  y_ += other.y_;

  return *this;
}

IntVector2D& IntVector2D::operator -= (const IntVector2D& other) {
  x_ -= other.x_;
  y_ -= other.y_;

  return *this;
}

IntVector2D& IntVector2D::operator *= (int scale) {
  x_ *= scale;
  y_ *= scale;

  return *this;
}

IntVector2D& IntVector2D::operator /= (int scale) {
  x_ /= scale;
  y_ /= scale;

  return *this;
}

IntVector2D& IntVector2D::operator /= (const IntVector2D& other) {
  x_ /= other.x_;
  y_ /= other.y_;

  return *this;
}

IntVector2D operator+ (IntVector2D v1, const IntVector2D& v2) {
  v1 += v2;
  return v1;
}

IntVector2D operator- (IntVector2D v1, const IntVector2D& v2) {
  v1 -= v2;
  return v1;
}

IntVector2D operator* (IntVector2D v, int scale) {
  v *= scale;
  return v;
}

IntVector2D operator* (int scale, IntVector2D v) {
  v *= scale;
  return v;
}

IntVector2D operator/ (IntVector2D v1, const IntVector2D& v2) {
  v1 /= v2;
  return v1;
}

IntVector2D operator/ (IntVector2D v, int scale) {
  v /= scale;
  return v;
}

IntVector2D operator/ (int scale, const IntVector2D& v) {
  IntVector2D vv(scale / v.x_, scale / v.y_);
  return vv;
}

std::ostream & operator<<(std::ostream &os, const IntVector2D& v) {
  os << "(x=" << v.X() << ", y=" << v.Y() << ")";

  return os;
}

#else
IntVector2D::IntVector2D(int x, int y)
   : Vector2D<int>(x, y) {}

IntVector2D::IntVector2D(const IntVector2D& other)
    : Vector2D<int>(other) {}
#endif

DoubleVector2D::DoubleVector2D(double x, double y)
   : Vector2D<double>(x, y) {}

DoubleVector2D::DoubleVector2D(const DoubleVector2D& other)
    : Vector2D<double>(other) {}

}  // namespace py

}  // namespace m2d
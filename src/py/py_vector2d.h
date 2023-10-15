#pragma once

#include "py/py.h"

#include <ostream>

//#include <boost/format.hpp>

namespace m2d {

namespace py {

template <typename T>
class Vector2D {
 public:
  Vector2D() = default;
  Vector2D(T x, T y)
    : x_(x)
    , y_(y) {}

  Vector2D(const Vector2D& other)
    : Vector2D(other.x_, other.y_) {}  

  T SquaredLength() const {
    return x_ * x_ + y_ * y_;
  }

  T Length() const {
    return std::sqrt(SquaredLength());
  }

  Vector2D MakeUnitVector() const {
    return *this / Length();
  }

  bool operator==(const Vector2D& other) const {
    if (this == &other) {
      return true;
    }

    return x_ == other.x_ && y_ == other.y_;
  }

  bool operator!=(const Vector2D& other) const {
    return !operator==(other);
  }

  Vector2D& operator += (const Vector2D& other) {
    x_ += other.x_;
    y_ += other.y_;

    return *this;
  }

  Vector2D& operator -= (const Vector2D& other) {
    x_ -= other.x_;
    y_ -= other.y_;

    return *this;
  }

  Vector2D& operator *= (T scale) {
    x_ *= scale;
    y_ *= scale;

    return *this;
  }

  Vector2D& operator /= (T scale) {
    x_ /= scale;
    y_ /= scale;

    return *this;
  }

  Vector2D& operator /= (const Vector2D& other) {
    x_ /= other.x_;
    y_ /= other.y_;

    return *this;
  }

  friend Vector2D operator+ (Vector2D v1, const Vector2D& v2) {
    v1 += v2;
    return v1;
  }

  friend Vector2D operator- (Vector2D v1, const Vector2D& v2) {
    v1 -= v2;
    return v1;
  }

  friend Vector2D operator* (Vector2D v, T scale) {
    v *= scale;
    return v;
  }

  friend Vector2D operator* (T scale, Vector2D v) {
    v *= scale;
    return v;
  }

  friend Vector2D operator/ (Vector2D v1, const Vector2D& v2) {
    v1 /= v2;
    return v1;
  }

  friend Vector2D operator/ (Vector2D v, T scale) {
    v /= scale;
    return v;
  }

  friend Vector2D operator/ (T scale, const Vector2D& v) {
    Vector2D vv(scale / v.x_, scale / v.y_);
    return vv;
  }

 //protected:
  T x_ = static_cast<T>(0);
  T y_ = static_cast<T>(0);
};

#if 1
class IntVector2D {
 public:
  static bp::object CreateClassDeclaration();

  IntVector2D() = default;
  IntVector2D(int x, int y);
  IntVector2D(const IntVector2D& other);
  int SquaredLength() const;
  int Length() const;
  IntVector2D MakeUnitVector() const;
  bool operator==(const IntVector2D& other) const;
  bool operator!=(const IntVector2D& other) const;
  IntVector2D& operator += (const IntVector2D& other);
  IntVector2D& operator -= (const IntVector2D& other);
  IntVector2D& operator *= (int scale);
  IntVector2D& operator /= (int scale);
  IntVector2D& operator /= (const IntVector2D& other);
  IntVector2D operator-() const;

  int X() const;
  int Y() const;

  friend IntVector2D operator+ (IntVector2D v1, const IntVector2D& v2);
  friend IntVector2D operator- (IntVector2D v1, const IntVector2D& v2);
  friend IntVector2D operator* (IntVector2D v, int scale);
  friend IntVector2D operator* (int scale, IntVector2D v);
  friend IntVector2D operator/ (IntVector2D v1, const IntVector2D& v2);
  friend IntVector2D operator/ (IntVector2D v, int scale);
  friend IntVector2D operator/ (int scale, const IntVector2D& v);
  //friend std::ostream & operator<<(std::ostream &os, const IntVector2D& v);

 private:
  int x_ = 0;
  int y_ = 0;
};
#else
class IntVector2D : public Vector2D<int> {
 public:
  static bp::object CreateClassDeclaration();

  IntVector2D() = default;
  IntVector2D(int x, int y);
  IntVector2D(const IntVector2D& other);
};
#endif

class DoubleVector2D : public Vector2D<double> {
 public:
  static bp::object CreateClassDeclaration();

  DoubleVector2D() = default;
  DoubleVector2D(double x, double y);
  DoubleVector2D(const DoubleVector2D& other);
};

std::ostream & operator<<(std::ostream &os, const IntVector2D& v);

}  // namespace py

}  // namespace m2d

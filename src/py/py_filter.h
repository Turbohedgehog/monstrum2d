#pragma once

#include "py/py.h"
#include "py/py_entity.h"

#include "common/common_types.h"

namespace m2d {

namespace py {

class Filter;

#if 0
class FilterIterator {
 public:
  FilterIterator(const ecs::FilterWeakPtr& filter, ecs::EntityMap::iterator entity_iterator);
  void operator++(int);
  void operator++();
  Entity operator*() const;
  Entity* operator->();

  friend bool operator== (const FilterIterator& a, const FilterIterator& b) { return a.entity_iterator_ == b.entity_iterator_; };
  friend bool operator!= (const FilterIterator& a, const FilterIterator& b) { return a.entity_iterator_ != b.entity_iterator_; };

 private:
  ecs::FilterWeakPtr filter_;
  ecs::EntityMap::iterator entity_iterator_;
};
#endif

class Filter {
 public:
  static bp::object CreateClassDeclaration();

  void SetFilter(ecs::FilterWeakPtr filter);
  //FilterIterator Begin();
  //FilterIterator End();
  Filter Iter();
  Entity Next();

 private:
  ecs::FilterWeakPtr filter_;
  ecs::EntityMap::const_iterator entity_iterator_;
};

}  // namespace py

}  // namespace m2d
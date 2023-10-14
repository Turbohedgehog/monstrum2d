#include "py/py_filter.h"

#include "ecs/entity.h"
#include "ecs/filter.h"

namespace m2d {

namespace py {

bp::object Filter::CreateClassDeclaration() {
  return bp::class_<Filter>("Filter")
      .def("__iter__", &Filter::Iter)
      .def("__next__", &Filter::Next)
  ;
}

void Filter::SetFilter(ecs::FilterWeakPtr filter) {
  filter_ = filter;
  entity_iterator_ = filter_.lock()->GetEnities().begin();
}

Filter Filter::Iter() {
  Filter filter;
  filter.SetFilter(filter_);

  return filter;
}

Entity Filter::Next() {
  auto& enities = filter_.lock()->GetEnities();
  for (; entity_iterator_ != enities.end();) {
    auto entity_ptr = entity_iterator_->second;
    if (entity_ptr.expired()) {
      entity_iterator_ = enities.erase(entity_iterator_);
    } else {
      ++entity_iterator_;

      Entity entity;
      entity.SetEntity(entity_ptr);

      return entity;
    }
  }
  
  bp::exec("raise StopIteration");

  return Entity();
}

}  // namespace py

}  // namespace m2d
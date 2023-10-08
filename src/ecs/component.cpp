#include "ecs/component.h"

namespace m2d {

namespace ecs {

Component::Component(std::size_t type_id, std::size_t enity_id, ComponentDataPtr data)
  : type_id_(type_id)
  , enity_id_(enity_id)
  , data_(data) {}

ComponentDataPtr Component::GetData() const {
  return data_;
}

bool Component::Tick(float delta) {
  if (!lifetime_) {
    return true;
  }

  bool res = true;

  std::visit(
    visitor_overload {
      [&res, &delta](double& timer) constexpr {
        timer -= delta;
        res = timer >= 0.;
      },
      [&res](int& ticks) constexpr {
        --ticks;
        res = ticks > 0;
      }
    },
    lifetime_.value()
  );

  return res;
}

}  // namespace ecs

}  // namespace m2d

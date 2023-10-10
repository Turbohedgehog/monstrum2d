#include "ecs/filter.h"

#include "ecs/ecs.h"
//#include "ecs/holder.h"
#include "ecs/entity.h"

namespace m2d {

namespace ecs {
  
Filter::Filter(ECSWeakPtr ecs, const ComponentBitmask& filter_bitmask)
    : ecs_(ecs)
    , filter_bitmask_(filter_bitmask) {}

bool Filter::IsValid() const {
  return filter_bitmask_.any();
}

void Filter::ProcessEntity(EntityWeakPtr entity) {
  auto entity_ptr = entity.lock();
  if ((filter_bitmask_ & entity_ptr->GetComponentBitmask()) == filter_bitmask_) {
    entities_[entity_ptr->GetId()] = entity_ptr;
  }
}


}  // namespace ecs

}  // namespace m2d
#include "ecs/system.h"

namespace m2d {

namespace ecs {

System::System(HolderWeakPtr holder, std::string system_name)
    : holder_(holder)
    , name_(std::move(system_name)) {}

const std::string& System::GetName() const {
  return name_;
}

HolderWeakPtr System::GetHolder() const {
  return holder_;
}

}  // namespace ecs

}  // namespace m2d
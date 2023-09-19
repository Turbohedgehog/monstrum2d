#include "ecs/holder.h"

namespace m2d {

namespace ecs {

Holder::Holder(std::size_t id)
  : id_(id) {}

void Holder::AppendComponentSchema(const std::filesystem::path& schema_path) {

}

void Holder::Update(float delta) {

}

std::size_t Holder::GetSystemCount() const {
  return system_count_;
}

} // namespace ecs

}  // namespace m2d

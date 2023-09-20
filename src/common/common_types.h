#pragma once

#include <memory>

namespace m2d {

namespace ecs {

class ECSHolder;
using ECSHolderPtr = std::shared_ptr<ECSHolder>;
using ECSHolderWeakPtr = std::weak_ptr<ECSHolder>;

class ECS;
using ECSPtr = std::shared_ptr<ECS>;
using ECSWeakPtr = std::weak_ptr<ECS>;

}  // namespace ecs

}  // namespace m2d

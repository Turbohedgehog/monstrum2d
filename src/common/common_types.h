#pragma once

#include <memory>

namespace m2d {

class ECSHandler;
using ECSHandlerPtr = std::shared_ptr<ECSHandler>;
using ECSHandlerWeakPtr = std::weak_ptr<ECSHandler>;

class ECS;
using ECSPtr = std::shared_ptr<ECS>;
using ECSWeakPtr = std::weak_ptr<ECS>;

}  // namespace m2d

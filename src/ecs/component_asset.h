#pragma once

#include "ecs/asset.h"

namespace m2d {

namespace ecs {
  
class IntegerFieldAsset {};

class FloatFieldAsset {};

class StringFieldAsset {};



class ComponentAsset : public Asset {
 public:
  bool Load(const std::string& filename) override;
};

}  // namespace ecs

}  // namespace m2d

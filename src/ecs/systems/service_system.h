#pragma once

#include "ecs/system.h"
#include "terminal/types.h"

namespace m2d {

namespace ecs {
  
class ServiceSystem : public System {
 public:
  ServiceSystem(HolderWeakPtr holder);

  void OnSystemRegistered() override;
  void Update(float delta) override;

 private:
  void UpdateKeypressed();

  hi::ScreenWeakPtr sceen_;
  ecs::ECSWeakPtr system_ecs_;
};

}  // namespace ecs

}  // namespace m2d

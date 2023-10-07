#pragma once

#include <string>

#include "ecs/ecs.h"

namespace m2d {

namespace ecs {
  
class System {
 public:
  System(HolderWeakPtr holder, std::string system_name);

  virtual void OnSystemRegistered() {}
  virtual void Update(float delta) {}

  const std::string& GetName() const;
  ECSWeakPtr GetECS() const;
  HolderWeakPtr GetHolder() const;

 private:
  HolderWeakPtr holder_;
  std::string name_;
};

}  // namespace ecs

}  // namespace m2d

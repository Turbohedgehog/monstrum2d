#include "ecs/systems/service_system.h"

#include "terminal/terminal.h"
#include "ecs/holder.h"

namespace m2d {

namespace ecs {

ServiceSystem::ServiceSystem(HolderWeakPtr holder)
    : System(holder, "ServiceSystem") {}

void ServiceSystem::OnSystemRegistered() {
  sceen_ = hi::Terminal::GetInstance().AppendScreen();
  system_ecs_ = GetHolder().lock()->GetOrCreateECS("SystemECS");
}

void ServiceSystem::Update(float delta) {
  System::Update(delta);

  UpdateKeypressed();
}

void ServiceSystem::UpdateKeypressed() {
  if (sceen_.expired()) {
    return;
  }
}

}  // namespace ecs

}  // namespace m2d

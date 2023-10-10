from Core import SystemBase
from Core import Terminal


class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

  def update(self, delta):
    super().update(delta)
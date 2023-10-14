from Core import SystemBase
from Core import Terminal


class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.gameplay_screen = Terminal.create_screen()
    self.holder = system_handler.get_holder()
    self.gameplay_ecs = self.holder.get_or_create_ecs("Gameplay")

    self.system_handler.enable_system_update(self)


  def update(self, delta):
    super().update(delta)
    

    key_pressed = self.gameplay_screen.get_key_pressed()
    if key_pressed is not None:
      if key_pressed == 27 or chr(key_pressed) == 'q':
        self.holder.shutdown()
    
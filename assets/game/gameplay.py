from Core import SystemBase
#from Core import *
#from Core1 import Terminal
#from Core import Terminal as term

#class Test(Terminal):
#  ...

#print(f"{Core.SystemBase.__module__} = {dir(Core.SystemBase.__module__)}")

class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    #print(SystemBase.__module__)

    self.gameplay_screen = Core.Terminal.create_screen()
    #self.gameplay_screen = Core.Terminal()
    self.holder = system_handler.get_holder()
    self.gameplay_ecs = self.holder.get_or_create_ecs("Gameplay")
    self.system_handler.enable_system_update(self)


  def update(self, delta):
    super().__init__(delta)
    
    key_pressed = self.gameplay_screen.get_key_pressed()
    #print(f"@@@ key_pressed = {key_pressed}")
    if key_pressed is not None:
      print(f"~~~~ key_pressed = {key_pressed}")
      if key_pressed == 27 or chr(key_pressed) == 'q':
        self.holder.shutdown()
    
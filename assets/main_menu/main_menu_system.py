from Core import SystemBase

class MainMenuSystem(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)
    
    print("Hello from MainMenuSystem!")
    self.update = True
    

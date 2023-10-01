from Core import SystemBase

class MainMenuSystem(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)
    
    print("Hello from MainMenuSystem!")
    #self.enable_update = True
    #system_handler.on_enable_update_property_changed(self)
    self.counter = 0
    self.time = 0.0
    #print("Roll back!")
    #self.enable_update = False
    self.holder = self.system_handler.get_holder()
    self.system_handler.enable_system_update(self)

  def update(self, delta):
    self.counter += 1
    self.time += delta
    print(f"[{self.counter}] {delta} -> {self.time}")

    if (self.counter >= 50):
      self.holder.shutdown()
      #self.system_handler.shutdown()
      #self.system_handler.disable_system_update(self)


print("Hello!")
    

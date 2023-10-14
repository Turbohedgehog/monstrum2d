from Core import SystemBase
from Core import Terminal



class Render(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()
    self.camera_location = (0, 0)

    self.map_schema = self.holder.get_component_schema("map")
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])

    self.gameplay_ecs = self.holder.get_or_create_ecs("Gameplay")
    self.gameplay_state_schema = self.holder.get_component_schema("gameplay_state")
    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])

    self.screen = None
    self.system_handler.enable_system_update(self)


  def update(self, delta):
    super().update(delta)
    self.draw_map()

  def check_screen(self):
    if self.screen is not None:
      return True
    
    for state_entity in self.gameplay_state_filter:
      gameplay_state = state_entity.get_component("gameplay_state")
      screen_id = self.gameplay_state_schema.get_field(gameplay_state, "screen_id")
      self.screen = Terminal.get_screen(screen_id)

    return self.screen is not None

  def draw_map(self):
    if not self.check_screen():
      return
    
    # Draw only first map
    for map_entity in self.map_filter:
      map_component = map_entity.get_component("map")
      width = self.map_schema.get_field(map_component, "width")
      height = self.map_schema.get_field(map_component, "height")

      for y in range(height):
        s = ''.join(chr(self.map_schema.get_field(map_component, ["tiles", x, y]) & 255) for x in range(width))
        self.screen.move_to(0, y)
        self.screen.print(s)
      break
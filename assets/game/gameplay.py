from Core import SystemBase
from Core import Terminal


class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.gameplay_screen = Terminal.create_screen()
    screen_id = self.gameplay_screen.get_id()
    self.holder = system_handler.get_holder()

    self.gameplay_ecs = self.holder.get_or_create_ecs("Gameplay")
    self.gameplay_state_schema = self.holder.get_component_schema("gameplay_state")
    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])

    self.map_schema = self.holder.get_component_schema("map")
    self.map_ecs = self.holder.get_or_create_ecs("map")

    gameplay_state_entity = self.gameplay_ecs.create_entity(["gameplay_state"])
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    self.gameplay_state_schema.set_field(gameplay_state, "screen_id", screen_id)

    self.system_handler.enable_system_update(self)
    self.create_map(40, 20)

  def update(self, delta):
    super().update(delta)

    key_pressed = self.gameplay_screen.get_key_pressed()
    if key_pressed is not None:
      if key_pressed == 27 or chr(key_pressed) == 'q':
        self.holder.shutdown()

  def create_map(self, width, height):
    map_entity = self.map_ecs.create_entity(["map"])
    map_component = map_entity.get_component("map")
    self.map_schema.set_field(map_component, "width", width)
    self.map_schema.set_field(map_component, "height", height)
    map_data = self.map_schema.get_field(map_component, "tiles")
    map_data.resize(width)
    for i in range(width):
      map_col = self.map_schema.get_field(map_component, ["tiles", i])
      map_col.resize(height)

    for x in range(width):
      for y in range(height):
        self.map_schema.set_field(map_component, ["tiles", x, y], ord(" "))

    wall = 1 << 8 | ord('#')
    wall2 = 1 << 8 | ord('@')
    for x in range(width):
      self.map_schema.set_field(map_component, ["tiles", x, 0], wall)
      self.map_schema.set_field(map_component, ["tiles", x, height - 1], wall)

    for y in range(1, height - 1):
      self.map_schema.set_field(map_component, ["tiles", 0, y], wall2)
      self.map_schema.set_field(map_component, ["tiles", width - 1, y], wall2)
    
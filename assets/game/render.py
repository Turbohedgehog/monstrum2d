from Core import SystemBase
from Core import Terminal
from Core import Color
from Core import IntVector2D


MAP_COLOR_PAIR = 1
SURVIVER_COLOR_PAIR = 2
BRUTE_COLOR_PAIR = 3
KEYS_COLOR_PAIRS = [4, 5, 6, 7,]

class Render(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()
    self.camera_location = (0, 0)

    self.map_schema = self.holder.get_component_schema("map")
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])

    self.gameplay_ecs = self.holder.get_or_create_ecs("gameplay")
    self.gameplay_state_schema = self.holder.get_component_schema("gameplay_state")
    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])

    self.screen = None
    self.system_handler.enable_system_update(self)

    self.coordinate_schema = self.holder.get_component_schema("coordinate")
    self.surviver_schema = self.holder.get_component_schema("surviver")
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate"])

    self.key_schema = self.holder.get_component_schema("key")
    self.key_filter = self.gameplay_ecs.get_or_create_filter(["key", "coordinate"])

    self.exit_schema = self.holder.get_component_schema("exit")
    self.exit_filter = self.gameplay_ecs.get_or_create_filter(["exit", "coordinate"])


  def update(self, delta):
    super().update(delta)

    self.draw()


  def draw(self):
    if not self.check_screen():
      return
    
    try:
      surviver = self.get_surviver()
      if surviver:
        self.screen.clear()
        surviver_location = self.get_surviver_location(surviver)
        screen_width = Terminal.get_width()
        screen_height = Terminal.get_height()
        size = IntVector2D(screen_width, screen_height)
        self.draw_map(surviver_location, size)
        self.draw_surviver(size)
        self.draw_keys(surviver_location, size)
    except Exception as ex:
      print(ex)
      raise

  def check_screen(self):
    if self.screen is not None:
      return True
    
    try:
      
      #print(f"type(Render) = {type(Render)}")
      #print(f"type(Color1) = {type(Color)}")
      #print(f"Color.__class__ = {Color.__class__}")
      #c = list(Color)
      #print(f"colors = {c}")
      #print(c)
      #print(f"colors = {[e.value for e in Color]}")
      for state_entity in self.gameplay_state_filter:
        gameplay_state = state_entity.get_component("gameplay_state")
        screen_id = self.gameplay_state_schema.get_field(gameplay_state, "screen_id")
        self.screen = Terminal.get_screen(screen_id)
      
      if not self.screen:
        return False
      
      #self.screen.set_color_pair(1, Color.White, Color.Black)
      # Scene
      self.screen.set_color_pair(MAP_COLOR_PAIR, 7, 0)
      # Surviver
      self.screen.set_color_pair(SURVIVER_COLOR_PAIR, 2, 0)
      # Brute
      self.screen.set_color_pair(BRUTE_COLOR_PAIR, 4, 0)
      # Key 1
      self.screen.set_color_pair(KEYS_COLOR_PAIRS[0], 1, 0)
      # Key 2
      self.screen.set_color_pair(KEYS_COLOR_PAIRS[1], 4, 0)
      # Key 3
      self.screen.set_color_pair(KEYS_COLOR_PAIRS[2], 5, 0)
      # Key 4
      self.screen.set_color_pair(KEYS_COLOR_PAIRS[3], 6, 0)

      self.screen.set_clear_color(MAP_COLOR_PAIR)
      self.screen.clear()
    except Exception as ex:
      print(ex)
      raise
    
    return True
  
  def get_surviver(self):
    return next((surviver for surviver in self.surviver_filter), None)
    
  def get_surviver_location(self, surviver_enity) -> IntVector2D:
    coordinate_component = surviver_enity.get_component("coordinate")
    x = self.coordinate_schema.get_field(coordinate_component, "x")
    y = self.coordinate_schema.get_field(coordinate_component, "y")
    return IntVector2D(x, y)

  def draw_surviver(self, screen_size: IntVector2D):
    center = screen_size / 2
    self.screen.move_to(center.x, center.y)
    self.screen.select_color_pair(SURVIVER_COLOR_PAIR)
    self.screen.print("@")

  def draw_keys(self, surviver_xy: IntVector2D, screen_size: IntVector2D):
    half_size = screen_size / 2
    for key_entity in self.key_filter:
      coordinate_component = key_entity.get_component("coordinate")
      x = self.coordinate_schema.get_field(coordinate_component, "x")
      y = self.coordinate_schema.get_field(coordinate_component, "y")
      key_pos = IntVector2D(x, y)
      diff = key_pos - surviver_xy
      if abs(diff.x) >= half_size.x or abs(diff.y) >= half_size.y:
        continue


      key_component = key_entity.get_component("key")
      id = self.key_schema.get_field(key_component, "id")

      screen_pos = half_size + diff
      self.screen.move_to(screen_pos.x, screen_pos.y)
      self.screen.select_color_pair(KEYS_COLOR_PAIRS[id])
      #self.screen.print("A")
      self.screen.print("¤")


  def draw_map(self, surviver_xy: IntVector2D, screen_size: IntVector2D):
    # Draw only first map
    map_entity = next((m for m in self.map_filter), None)
    if map_entity is None:
      return
    
    screen_width = screen_size.x
    screen_height = screen_size.y
    screen_size_half = screen_size / 2
    #screen_width_half = int(screen_width / 2)
    #screen_height_half = int(screen_height / 2)
    
    
    map_component = map_entity.get_component("map")
    map_width = self.map_schema.get_field(map_component, "width")
    map_height = self.map_schema.get_field(map_component, "height")

    screen_width_from = surviver_xy.x - screen_size_half.x
    screen_height_from = surviver_xy.y - screen_size_half.y
    screen_width_to = screen_width_from + screen_width
    #screen_height_to = screen_height_from + screen_height

    self.screen.select_color_pair(MAP_COLOR_PAIR)
    for y in range(screen_height):
      map_y = screen_height_from + y
      if map_y < 0 or map_y >= map_height:
        s = " " * screen_width
      else:
        s = ""
        for x in range(screen_width_from, screen_width_to):
          if x < 0 or x >= map_width:
            s += " "
          else:
            s += chr(self.map_schema.get_field(map_component, ["tiles", x, map_y]) & 255)
      self.screen.move_to(0, y)
      self.screen.print(s)

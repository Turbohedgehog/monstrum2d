from Core import SystemBase
from Core import Terminal


#TERMINAL_SIZE = (120, 30)
#TERMINAL_SIZE_HALF = TERMINAL_SIZE / 2

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
    self.bound_schema = self.holder.get_component_schema("bound")
    self.surviver_schema = self.holder.get_component_schema("surviver")
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate", "bound"])


  def update(self, delta):
    super().update(delta)

    self.draw()


  def draw(self):
    if not self.check_screen():
      return
    
    try:
      #self.screen.clear()
      surviver = self.get_surviver()
      if surviver:
        x, y = self.get_surviver_location(surviver)
        screen_width = Terminal.get_width()
        screen_height = Terminal.get_height()
        size = (screen_width, screen_height)
        self.draw_map((x, y), size)
        self.draw_surviver(size)
    except Exception as ex:
      print(ex)
      raise

  def check_screen(self):
    if self.screen is not None:
      return True
    
    for state_entity in self.gameplay_state_filter:
      gameplay_state = state_entity.get_component("gameplay_state")
      screen_id = self.gameplay_state_schema.get_field(gameplay_state, "screen_id")
      self.screen = Terminal.get_screen(screen_id)

    return self.screen is not None
  
  def get_surviver(self):
    return next((surviver for surviver in self.surviver_filter), None)
    
  def get_surviver_location(self, surviver_enity):
    coordinate_component = surviver_enity.get_component("coordinate")
    x = self.coordinate_schema.get_field(coordinate_component, "x")
    y = self.coordinate_schema.get_field(coordinate_component, "y")
    return x, y

  def draw_surviver(self, screen_size: tuple):
    self.screen.move_to(int(screen_size[0] / 2), int(screen_size[1] / 2))
    self.screen.print("@")

  def draw_map(self, surviver_xy: tuple, screen_size: tuple):
    # Draw only first map
    map_entity = next((m for m in self.map_filter), None)
    if map_entity is None:
      return
    
    screen_width = screen_size[0]
    screen_height = screen_size[1]
    screen_width_half = int(screen_width / 2)
    screen_height_half = int(screen_height / 2)
    
    #start_draw_loc = surviver_xy - screen_size_half
    #end_draw_loc = start_draw_loc + screen_size
    
    map_component = map_entity.get_component("map")
    map_width = self.map_schema.get_field(map_component, "width")
    map_height = self.map_schema.get_field(map_component, "height")

    screen_width_from = surviver_xy[0] - screen_width_half
    screen_height_from = surviver_xy[1] - screen_height_half
    screen_width_to = screen_width_from + screen_width
    #screen_height_to = screen_height_from + screen_height

    
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

    #self.screen.move_to(10, 10)
    #self.screen.print("1")
    #self.screen.refresh()


    '''
    for y in range(height):
      s = ''.join(chr(self.map_schema.get_field(map_component, ["tiles", x, y]) & 255) for x in range(width))
      self.screen.move_to(0, y)
      self.screen.print(s)
    '''
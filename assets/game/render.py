from Core import SystemBase
from Core import Terminal
from Core import IntVector2D


MAP_COLOR_PAIR = 1
BAD_ENDING_COLOR_PAIR = MAP_COLOR_PAIR + 1
SURVIVER_COLOR_PAIR = BAD_ENDING_COLOR_PAIR + 1
BRUTE_COLOR_PAIR = SURVIVER_COLOR_PAIR + 1
KEYS_COLOR_PAIRS = [BRUTE_COLOR_PAIR + i + 1 for i in range(4)]

class Render(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()
    self.camera_location = (0, 0)

    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])
    self.gameplay_ecs = self.holder.get_or_create_ecs("gameplay")
    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])
    self.gameplay_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate"])
    self.key_filter = self.gameplay_ecs.get_or_create_filter(["key", "coordinate"])
    self.exit_filter = self.gameplay_ecs.get_or_create_filter(["exit", "coordinate"])
    self.brute_filter = self.gameplay_ecs.get_or_create_filter(["brute", "coordinate"])

    self._screen = None
    self.time = 0

    self.exit_border = [
      IntVector2D(-1, -1), IntVector2D(0, -1), IntVector2D(1, -1),
      IntVector2D(-1, 0), IntVector2D(1, 0),
      IntVector2D(-1, 1), IntVector2D(0, 1), IntVector2D(1, 1),
    ]

  def get_gameplay(self):
    return next((gameplay for gameplay in self.gameplay_filter), None)

  def update(self, delta):
    super().update(delta)

    self.time += delta

    self.draw()

  def draw(self):
    gameplay = self.get_gameplay()
    if not gameplay:
      return
    
    screen = self.init_or_get_screen()
    if not screen:
      return
    
    try:
      gameplay_state = gameplay.get_component("gameplay_state")
      state = gameplay_state.get_field("state")
      if state == 0:
        self.draw_intro(screen)
      elif state == 1:
        self.draw_gameplay(screen)
      elif state == 2:
        self.draw_good_ending(screen)
      elif state == 3:
        self.draw_bad_ending(screen)
      else:
        raise "Unexpected gameplay state"
      #self.draw_time(screen)
    except Exception as ex:
      print(ex)
      raise


  def draw_intro(self, screen):
    title = "Remember! You're not alone..."
    center = Terminal.get_size() / 2
    
    screen.clear()

    screen.move_to(int(center.x - len(title) / 2), center.y - 1)
    screen.print(title)

  def draw_gameplay(self, screen):
    try:
      surviver = self.get_surviver()
      if surviver:
        screen.clear()
        surviver_location = self.get_surviver_location(surviver)
        self.draw_map(screen, surviver_location)
        self.draw_exit(screen, surviver_location)
        self.draw_keys(screen, surviver_location)
        self.draw_surviver(screen)
        self.draw_brute(screen, surviver_location)
    except Exception as ex:
      print(ex)
      raise

  def draw_good_ending(self, screen):
    title = "Congratulations!!!"
    message = "You have left the dungeon!"
    center = Terminal.get_size() / 2

    screen.select_color_pair(MAP_COLOR_PAIR)
    
    screen.clear()

    screen.move_to(int(center.x - len(title) / 2), center.y - 1)
    screen.print(title)

    screen.move_to(int(center.x - len(message) / 2), center.y + 1)
    screen.print(message)

  def draw_bad_ending(self, screen):
    message = "    POTRA4ENO!!!    "
    message_len = len(message)
    border = " " * message_len
    center = Terminal.get_size() / 2

    screen.set_clear_color(BAD_ENDING_COLOR_PAIR)
    screen.select_color_pair(BRUTE_COLOR_PAIR)
    screen.clear()

    x = int(center.x - message_len / 2)
    screen.move_to(x, center.y - 1)
    screen.print(border)
    screen.move_to(x, center.y)
    screen.print(message)
    screen.move_to(x, center.y + 1)
    screen.print(border)

  def init_or_get_screen(self):
    if self._screen is not None:
      return self._screen
    
    try:
      for state_entity in self.gameplay_state_filter:
        gameplay_state = state_entity.get_component("gameplay_state")
        screen_id = gameplay_state.get_field("screen_id")
        self._screen = Terminal.get_screen(screen_id)
      
      if not self._screen:
        return None
      
      # Scene
      self._screen.set_color_pair(MAP_COLOR_PAIR, 7, 0)
      # Surviver
      self._screen.set_color_pair(SURVIVER_COLOR_PAIR, 2, 0)
      # Brute
      self._screen.set_color_pair(BRUTE_COLOR_PAIR, 4, 0)
      # Bad ending
      self._screen.set_color_pair(BAD_ENDING_COLOR_PAIR, 0, 4)
      # Key 1
      self._screen.set_color_pair(KEYS_COLOR_PAIRS[0], 1, 0)
      # Key 2
      self._screen.set_color_pair(KEYS_COLOR_PAIRS[1], 4, 0)
      # Key 3
      self._screen.set_color_pair(KEYS_COLOR_PAIRS[2], 5, 0)
      # Key 4
      self._screen.set_color_pair(KEYS_COLOR_PAIRS[3], 6, 0)

      self._screen.set_clear_color(MAP_COLOR_PAIR)
      self._screen.clear()
    except Exception as ex:
      print(ex)
      raise
    
    return self._screen
  
  def get_surviver(self):
    return next((surviver for surviver in self.surviver_filter), None)
    
  def get_surviver_location(self, surviver_enity) -> IntVector2D:
    coordinate_component = surviver_enity.get_component("coordinate")
    x = coordinate_component.get_field("x")
    y = coordinate_component.get_field("y")
    return IntVector2D(x, y)

  def draw_surviver(self, screen):
    center = Terminal.get_size() / 2
    screen.move_to(center.x, center.y)
    screen.select_color_pair(SURVIVER_COLOR_PAIR)
    screen.print("@")

  def draw_brute(self, screen, surviver_xy: IntVector2D):
    screen_size = Terminal.get_size()
    half_size = screen_size / 2
    for brute_entity in self.brute_filter:
      coordinate_component = brute_entity.get_component("coordinate")
      x = coordinate_component.get_field("x")
      y = coordinate_component.get_field("y")
      brute_coordinate = IntVector2D(x, y)
      diff = brute_coordinate - surviver_xy
      if abs(diff.x) >= half_size.x or abs(diff.y) >= half_size.y:
        continue
    
      screen_pos = half_size + diff
      screen.move_to(screen_pos.x, screen_pos.y)
      screen.select_color_pair(BRUTE_COLOR_PAIR)
      screen.print("M")

  def draw_exit(self, screen, surviver_xy: IntVector2D):
    map_entity = self.get_map()
    if not map_entity:
      return
    screen_size = Terminal.get_size()
    map_component = map_entity.get_component("map")
    half_size = screen_size / 2
    for exit_entity in self.exit_filter:
      exit_coordinate_component = exit_entity.get_component("coordinate")
      x = exit_coordinate_component.get_field("x")
      y = exit_coordinate_component.get_field("y")
      exit_pos = IntVector2D(x, y)
      diff = exit_pos - surviver_xy
      screen_pos = half_size + diff
      screen_pos.x = max(0, min(screen_size.x - 1, screen_pos.x))
      screen_pos.y = max(0, min(screen_size.y - 1, screen_pos.y))

      exit_component = exit_entity.get_component("exit")
      is_opened = exit_component.get_field("state") > 0
      color = SURVIVER_COLOR_PAIR if is_opened else BRUTE_COLOR_PAIR
      screen.select_color_pair(color)

      for b in self.exit_border:
        p = screen_pos + b
        if p.x < 0 or p.x >= screen_size.x:
          continue
        if p.y < 0 or p.y >= screen_size.y:
          continue
        border_pos = exit_pos + b
        if (map_component.get_field(["tiles", border_pos.x, border_pos.y]) & ~255) != 0:
          continue
        screen.move_to(p.x, p.y)
        screen.print("G")
      if not is_opened:
        screen.move_to(screen_pos.x, screen_pos.y)
        screen.print("X")


  def draw_keys(self, screen, surviver_xy: IntVector2D):
    screen_size = Terminal.get_size()
    half_size = screen_size / 2
    for key_entity in self.key_filter:
      coordinate_component = key_entity.get_component("coordinate")
      x = coordinate_component.get_field("x")
      y = coordinate_component.get_field("y")
      key_pos = IntVector2D(x, y)
      diff = key_pos - surviver_xy
      '''
      if abs(diff.x) >= half_size.x or abs(diff.y) >= half_size.y:
        continue
      '''
      key_component = key_entity.get_component("key")
      id = key_component.get_field("id")

      screen_pos = half_size + diff
      screen_pos.x = max(0, min(screen_size.x - 1, screen_pos.x))
      screen_pos.y = max(0, min(screen_size.y - 1, screen_pos.y))
      screen.move_to(screen_pos.x, screen_pos.y)
      screen.select_color_pair(KEYS_COLOR_PAIRS[id])
      screen.print("K")

  def draw_time(self, screen):
    screen.move_to(0, 0)
    screen.print(f"   {self.time}   ")

  def get_map(self):
    return next((m for m in self.map_filter), None)

  def draw_map(self, screen, surviver_xy: IntVector2D):
    # Draw only first map
    map_entity = self.get_map()
    if map_entity is None:
      return
    
    screen_size = Terminal.get_size()
    screen_width = screen_size.x
    screen_height = screen_size.y
    screen_size_half = screen_size / 2
    
    map_component = map_entity.get_component("map")
    map_width = map_component.get_field("width")
    map_height = map_component.get_field("height")

    screen_width_from = surviver_xy.x - screen_size_half.x
    screen_height_from = surviver_xy.y - screen_size_half.y
    screen_width_to = screen_width_from + screen_width

    screen.select_color_pair(MAP_COLOR_PAIR)
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
            s += chr(map_component.get_field(["tiles", x, map_y]) & 255)
      screen.move_to(0, y)
      screen.print(s)

from Core import SystemBase
from Core import Terminal
from enum import Enum


class MoveDirection(Enum):
  STOP = 0
  UP = 1
  RIGHT = 2
  DOWN = 3
  LEFT = 4

class TileType(Enum):
  GROUND = 0
  WALL = 1 << 8

MOVE_TIME = 0.05

class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    Terminal.set_size(120, 30)

    self.gameplay_screen = Terminal.create_screen()
    screen_id = self.gameplay_screen.get_id()
    self.holder = system_handler.get_holder()

    self.gameplay_ecs = self.holder.get_or_create_ecs("gameplay")

    self.gameplay_state_schema = self.holder.get_component_schema("gameplay_state")
    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])

    self.map_schema = self.holder.get_component_schema("map")
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])

    gameplay_state_entity = self.gameplay_ecs.create_entity(["gameplay_state"])
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    self.gameplay_state_schema.set_field(gameplay_state, "screen_id", screen_id)

    self.coordinate_schema = self.holder.get_component_schema("coordinate")
    self.bound_schema = self.holder.get_component_schema("bound")
    self.surviver_schema = self.holder.get_component_schema("surviver")
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate", "bound"])

    self.system_handler.enable_system_update(self)

    #self.create_map(200, 200)
    self.create_map(40, 20)
    self.create_surviver()

  def update(self, delta):
    super().update(delta)

    self.update_input(delta)

  def create_surviver(self):
    surviver = self.gameplay_ecs.create_entity(["surviver", "coordinate", "bound"])
    coordinate_component = surviver.get_component("coordinate")
    self.coordinate_schema.set_field(coordinate_component, "x", 5)
    self.coordinate_schema.set_field(coordinate_component, "y", 5)

  def update_input(self, delta):
    key_pressed = self.gameplay_screen.get_key_pressed()
    if key_pressed is not None:
      if key_pressed == 27 or chr(key_pressed) == 'q':
        self.holder.shutdown()

    self.update_player_movement(delta, key_pressed)

  def update_player_movement(self, delta, key_pressed):
    # 452 - left
    # 454 - right
    # 450 - up
    # 456 - down

    surviver_enity = next((surviver for surviver in self.surviver_filter), None)
    if surviver_enity is None:
      return
    
    map_entity = next((m for m in self.map_filter), None) 
    if map_entity is None:
      return
    
    try:
      surviver_component = surviver_enity.get_component("surviver")
      key_down_timer = self.surviver_schema.get_field(surviver_component, "key_down_timer")
      move_direction = self.surviver_schema.get_field(surviver_component, "move_direction")

      def check_and_apply_move_direction(direction: MoveDirection):
        nonlocal key_down_timer
        nonlocal move_direction
        if move_direction != direction.value:
          key_down_timer = MOVE_TIME
          move_direction = direction.value
        else:
          key_down_timer += delta

      if key_pressed is None:
        key_down_timer = 0.0
        move_direction = MoveDirection.STOP.value
      elif key_pressed == 452 or chr(key_pressed) == "a":
        check_and_apply_move_direction(MoveDirection.LEFT)
      elif key_pressed == 450 or chr(key_pressed) == "w":
        check_and_apply_move_direction(MoveDirection.UP)
      elif key_pressed == 454 or chr(key_pressed) == "d":
        check_and_apply_move_direction(MoveDirection.RIGHT)
      elif key_pressed == 456 or chr(key_pressed) == "s":
        check_and_apply_move_direction(MoveDirection.DOWN)
      else:
        key_down_timer = 0.0
        move_direction = MoveDirection.STOP.value

      self.surviver_schema.set_field(surviver_component, "move_direction", move_direction)
      self.surviver_schema.set_field(surviver_component, "key_down_timer", key_down_timer)

      if key_down_timer >= MOVE_TIME:
        key_down_timer -= MOVE_TIME
        self.surviver_schema.set_field(surviver_component, "key_down_timer", key_down_timer)
        coordinate_component = surviver_enity.get_component("coordinate")
        x = self.coordinate_schema.get_field(coordinate_component, "x")
        y = self.coordinate_schema.get_field(coordinate_component, "y")

        map_component = map_entity.get_component("map")
        width = self.map_schema.get_field(map_component, "width")
        height = self.map_schema.get_field(map_component, "height")
        self.surviver_schema.set_field(surviver_component, "key_down_timer", key_down_timer)

        # Check map bound
        if move_direction == MoveDirection.LEFT.value:
          x -= 1
          if x < 0:
            return
        elif move_direction == MoveDirection.UP.value:
          y -= 1
          if y < 0:
            return
        elif move_direction == MoveDirection.RIGHT.value:
          x += 1
          if x >= width:
            return
        elif move_direction == MoveDirection.DOWN.value:
          y += 1
          if y >= height:
            return
          
        # Check wall
        tile = self.map_schema.get_field(map_component, ["tiles", x, y])
        if tile & TileType.WALL.value != 0:
          return
        
        self.coordinate_schema.set_field(coordinate_component, "x", x)
        self.coordinate_schema.set_field(coordinate_component, "y", y)
    except Exception as ex:
      print(f"~ {ex}")
      raise



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

    wall = TileType.WALL.value | ord('#')
    #wall2 = TileType.WALL.value | ord('@')
    for x in range(width):
      self.map_schema.set_field(map_component, ["tiles", x, 0], wall)
      self.map_schema.set_field(map_component, ["tiles", x, height - 1], wall)

    for y in range(1, height - 1):
      self.map_schema.set_field(map_component, ["tiles", 0, y], wall)
      self.map_schema.set_field(map_component, ["tiles", width - 1, y], wall)
      #self.map_schema.set_field(map_component, ["tiles", 0, y], TileType.WALL.value | ord('a') + y)
      #self.map_schema.set_field(map_component, ["tiles", width - 1, y], TileType.WALL.value | ord('a') + y)
    
import random

from enum import Enum

#import locale

from Core import SystemBase
from Core import Terminal
from Core import IntVector2D

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
CORRIDOR_BRANCHE_COUNT = (1, 3)
CORRIDOR_LENGHT = (5, 15)
ZERO_VECTOR = IntVector2D(0, 0)
KEY_COUNT = 4
#KEY_COUNT = 1

BORDER_SIZE = 1

def sign(value):
  if value > 0:
    return 1
  if value < 0:
    return -1
  return 0

def create_dir_vector(dir: IntVector2D):
  return IntVector2D(sign(dir.x), sign(dir.y))

class Gameplay(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    #print(f"locale = {locale.getlocale()}")

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
    self.gameplay_state_schema.set_field(gameplay_state, "state", 1)
    self.gameplay_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])

    self.coordinate_schema = self.holder.get_component_schema("coordinate")
    #self.bound_schema = self.holder.get_component_schema("bound")
    self.surviver_schema = self.holder.get_component_schema("surviver")
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate"])

    self.key_schema = self.holder.get_component_schema("key")
    self.key_filter = self.gameplay_ecs.get_or_create_filter(["key", "coordinate"])

    self.exit_schema = self.holder.get_component_schema("exit")
    self.exit_filter = self.gameplay_ecs.get_or_create_filter(["exit", "coordinate"])

    self.system_handler.enable_system_update(self)

    try:
      #map_size = IntVector2D(300, 150)
      map_size = IntVector2D(100, 100)
      spawn_pos = map_size / 2
      maze, start_points = Gameplay.create_maze(map_size, IntVector2D(8, 4), IntVector2D(20, 10))
      self.create_keys(start_points, KEY_COUNT)
      #self.create_keys([spawn_pos, spawn_pos, spawn_pos, spawn_pos], KEY_COUNT)
      
      self.create_exit(spawn_pos)
      Gameplay.cleanup_maze(maze)
      self.create_map(maze)
      self.create_surviver(spawn_pos)
    except Exception as ex:
      print(ex)
      raise

  def update(self, delta):
    super().update(delta)

    self.update_input(delta)

  def create_keys(self, start_points, count):
    count = min(count, len(start_points))
    if count == 0:
      return
    
    key_points = random.sample(start_points, count)
    for i, key_point in enumerate(key_points):
      print(f"key[{i}] = {key_point.x, key_point.y}")
      key_entity = self.gameplay_ecs.create_entity(["key", "coordinate"])
      print(f"key_entity = {key_entity.get_id()}")

      coordinate_component = key_entity.get_component("coordinate")
      self.coordinate_schema.set_field(coordinate_component, "x", key_point.x)
      self.coordinate_schema.set_field(coordinate_component, "y", key_point.y)

      key_component = key_entity.get_component("key")
      self.key_schema.set_field(key_component, "id", i)

  def create_exit(self, position: IntVector2D):
    exit = self.gameplay_ecs.create_entity(["exit", "coordinate"])
    coordinate_component = exit.get_component("coordinate")
    self.coordinate_schema.set_field(coordinate_component, "x", position.x)
    self.coordinate_schema.set_field(coordinate_component, "y", position.y)

  def create_surviver(self, spawn_pos: IntVector2D):
    surviver = self.gameplay_ecs.create_entity(["surviver", "coordinate"])
    coordinate_component = surviver.get_component("coordinate")
    self.coordinate_schema.set_field(coordinate_component, "x", spawn_pos.x)
    self.coordinate_schema.set_field(coordinate_component, "y", spawn_pos.y)

  def update_input(self, delta):
    key_pressed = self.gameplay_screen.get_key_pressed()
    if key_pressed is not None:
      #if key_pressed == 27 or chr(key_pressed) == 'q':
      if key_pressed == 27:
        self.holder.shutdown()

    gameplay_state_entity = next((gameplay for gameplay in self.gameplay_filter), None)
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    state = self.gameplay_state_schema.get_field(gameplay_state, "state")
    if state == 0:
      self.gameplay_state_schema.set_field(gameplay_state, "state", 1)
    elif state == 1:
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
        
        surviver_pos = IntVector2D(x, y)

        self.check_key_pickup(surviver_pos, surviver_component)
        self.check_exit(surviver_pos, surviver_component)

    except Exception as ex:
      print(f"~ {ex}")
      raise
  
  def check_key_pickup(self, surviver_pos: IntVector2D, surviver_component):
    for key_entity in self.key_filter:
      key_coordinate_component = key_entity.get_component("coordinate")
      key_x = self.coordinate_schema.get_field(key_coordinate_component, "x")
      key_y = self.coordinate_schema.get_field(key_coordinate_component, "y")

      if key_x == surviver_pos.x and key_y == surviver_pos.y:
        self.gameplay_ecs.remove_entity(key_entity.get_id())
        collected_keys = self.surviver_schema.get_field(surviver_component, "collected_keys")
        collected_keys += 1
        self.surviver_schema.set_field(surviver_component, "collected_keys", collected_keys)
        if collected_keys >= KEY_COUNT:
          for exit_entity in self.exit_filter:
            exit_component = exit_entity.get_component("exit")
            self.exit_schema.set_field(exit_component, "state", 1)
        break

  def check_exit(self, surviver_pos: IntVector2D, surviver_component):
    collected_keys = self.surviver_schema.get_field(surviver_component, "collected_keys")
    if collected_keys < KEY_COUNT:
      return
    
    for exit_entity in self.exit_filter:
      exit_coordinate = exit_entity.get_component("coordinate")
      exit_x = self.coordinate_schema.get_field(exit_coordinate, "x")
      exit_y = self.coordinate_schema.get_field(exit_coordinate, "y")
      if surviver_pos.x == exit_x and surviver_pos.y == exit_y:
        for gameplay_state_entity in self.gameplay_filter:
          gameplay_state = gameplay_state_entity.get_component("gameplay_state")
          # Good ending
          self.gameplay_state_schema.set_field(gameplay_state, "state", 2)


  @staticmethod
  def iterate_maze(start_pos: IntVector2D, maze_size: IntVector2D, maze_data, min_room_size: IntVector2D, max_room_size: IntVector2D):
    max_path_len = 20
    turtle_moves = 100
    point = start_pos
    move_directions = [
      IntVector2D(1, 0),
      IntVector2D(0, 1),
      IntVector2D(-1, 0),
      IntVector2D(0, -1),
    ]

    direction_idx = -1

    def create_room(pos: IntVector2D):
      room_with = random.randint(min_room_size.x, max_room_size.x)
      room_height = random.randint(min_room_size.y, max_room_size.y)
      size = IntVector2D(room_with, room_height)

      left_top_corner = pos - size / 2
      left_top_corner.x = max(BORDER_SIZE, left_top_corner.x)
      left_top_corner.y = max(BORDER_SIZE, left_top_corner.y)

      right_down_corner = pos + size / 2
      right_down_corner.x = min(maze_size.x - 1, right_down_corner.x - BORDER_SIZE)
      right_down_corner.y = min(maze_size.y - 1, right_down_corner.y - BORDER_SIZE)

      for x in range(left_top_corner.x, right_down_corner.x):
        for y in range(left_top_corner.y, right_down_corner.y):
          maze_data[x][y] = " "
      

    for _ in range(turtle_moves):
      new_direction_idx = random.randint(0, 3)
      if new_direction_idx == direction_idx:
        new_direction_idx += random.randint(-1, 1)
      new_direction_idx %= 4
      direction_idx = new_direction_idx
      dir = move_directions[direction_idx]
      path_len = random.randint(1, max_path_len)
      
      for _ in range(path_len):
        maze_data[point.x][point.y] = " "
        point += dir
        if point.x >= maze_size.x - BORDER_SIZE or point.x < BORDER_SIZE:
          dir.x = -dir.x
          point.x = max(BORDER_SIZE, min(point.x, maze_size.x - BORDER_SIZE))
          point.x += dir.x
          break
        if point.y >= maze_size.y - BORDER_SIZE or point.y < BORDER_SIZE:
          dir.y = - dir.y
          point.y = max(BORDER_SIZE, min(point.y, maze_size.y - BORDER_SIZE))
          point.y += dir.y
          break

      if random.randint(0, 20) == 0:
        create_room(point)

  @staticmethod
  def create_maze(map_size: IntVector2D, min_room_size: IntVector2D, max_room_size: IntVector2D):
    map_data = [None] * map_size.x
    for x in range(map_size.x):
      map_data[x] = ["#"] * map_size.y

    start_pos = map_size / 2
    start_points = []
    for _ in range(30):
      Gameplay.iterate_maze(start_pos, map_size, map_data, min_room_size, max_room_size)
      start_pos.x = random.randint(BORDER_SIZE, map_size.x - 1 - BORDER_SIZE)
      start_pos.y = random.randint(BORDER_SIZE, map_size.y - 1 - BORDER_SIZE)
      start_points.append(IntVector2D(start_pos.x, start_pos.y))

    return map_data, start_points
  
  @staticmethod
  def cleanup_maze(maze_data):
    width = len(maze_data)
    if width == 0:
      return
    height = len(maze_data[0])
    if height == 0:
      return
    
    space_check = [
      IntVector2D(-1, -1), IntVector2D(0, -1), IntVector2D(1, -1),
      IntVector2D(-1, 0), IntVector2D(1, 0),
      IntVector2D(-1, 1), IntVector2D(0, 1), IntVector2D(1, 1),
      ]
    def has_any_space(point: IntVector2D):
      for check in space_check:
        c = point + check
        if c.x < 0 or c.x >= width:
          continue
        if c.y < 0 or c.y >= width:
          continue
        if maze_data[c.x][c.y] == " ":
          return True
      return False


    for x in range(BORDER_SIZE, width - 1 - BORDER_SIZE):
      for y in range(BORDER_SIZE, height - 1 - BORDER_SIZE):
        if not has_any_space(IntVector2D(x, y)):
          maze_data[x][y] = "~"

  def create_map(self, maze_data):
    width = len(maze_data)
    if width == 0:
      return
    height = len(maze_data[0])
    if height == 0:
      return
    
    map_entity = self.map_ecs.create_entity(["map"])
    map_component = map_entity.get_component("map")
    self.map_schema.set_field(map_component, "width", width)
    self.map_schema.set_field(map_component, "height", height)
    map_data = self.map_schema.get_field(map_component, "tiles")
    map_data.resize(width)

    for i in range(width):
      map_col = self.map_schema.get_field(map_component, ["tiles", i])
      map_col.resize(height)

    space = ord(" ")
    #wall = TileType.WALL.value | ord('#')
    for x in range(width):
      for y in range(height):
        value = maze_data[x][y]
        tile = space if value == " " else TileType.WALL.value | ord(value)
        self.map_schema.set_field(map_component, ["tiles", x, y], tile)
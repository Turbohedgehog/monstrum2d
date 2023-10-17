import random

from enum import Enum

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

    #self.create_maze(1000, 1000, 4, 10)
    try:
      map_size = IntVector2D(300, 300)
      #map_size = IntVector2D(40, 40)
      maze = Gameplay.create_maze(map_size, IntVector2D(4, 4), IntVector2D(10, 10))
      #self.create_maze(IntVector2D(120, 30), IntVector2D(4, 4), IntVector2D(10, 10))
      #self.create_map(200, 200)
      #self.create_map(40, 20)
      self.create_map(maze)
      self.create_surviver(map_size)
    except Exception as ex:
      print(ex)
      raise

  def update(self, delta):
    super().update(delta)

    self.update_input(delta)

  def create_surviver(self, map_size):
    surviver = self.gameplay_ecs.create_entity(["surviver", "coordinate", "bound"])
    coordinate_component = surviver.get_component("coordinate")
    spawn_pos = map_size / 2
    self.coordinate_schema.set_field(coordinate_component, "x", spawn_pos.x)
    self.coordinate_schema.set_field(coordinate_component, "y", spawn_pos.y)

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
  
  @staticmethod
  def iterate_maze(maze_size: IntVector2D, maze_data):
    max_path_len = 20
    turtle_moves = 100
    point = maze_size / 2
    move_directions = [
      IntVector2D(1, 0),
      IntVector2D(0, 1),
      IntVector2D(-1, 0),
      IntVector2D(0, -1),
    ]

    direction_idx = -1

    for _ in range(turtle_moves):
      new_direction_idx = random.randint(0, 3)
      if new_direction_idx == direction_idx:
        new_direction_idx += random.randint(-1, 1)
      new_direction_idx %= 4
      direction_idx = new_direction_idx
      dir = move_directions[direction_idx]
      path_len = random.randint(1, max_path_len)
      
      #print(f"direction_idx = {direction_idx}")
      #print(f"path_len = {path_len}")
      #print(f"dir = {dir.x, dir.y}")
      for _ in range(path_len):
        #print(f"point = {point.x, point.y}")
        maze_data[point.x][point.y] = " "
        point += dir
        if point.x >= maze_size.x or point.x < 0:
          dir.x = -dir.x
          point.x = max(0, min(point.x, maze_size.x))
          point.x += dir.x
          break
        if point.y >= maze_size.y or point.y < 0:
          dir.y = - dir.y
          point.y = max(0, min(point.y, maze_size.y))
          point.y += dir.y
          break
  @staticmethod
  def create_maze(map_size: IntVector2D, min_room_size: IntVector2D, max_room_size: IntVector2D):
    #map_data = [["#"] * map_size.y] * map_size.x
    map_data = [None] * map_size.x
    for x in range(map_size.x):
      map_data[x] = ['#'] * map_size.y

    for _ in range(10):
      Gameplay.iterate_maze(map_size, map_data)

    '''
    for m in map_data:
      s = "".join(m)
      print(s)
    '''
    return map_data

  @staticmethod
  def create_maze_3(map_size: IntVector2D, min_room_size: IntVector2D, max_room_size: IntVector2D):
    map_data = [["#"] * map_size.y] * map_size.x
    #branch_count = 200
    branch_count = 1
    def turn_dir_left(dir: IntVector2D):
      return IntVector2D(-dir.y, dir.x)
  
    def turn_dir_right(dir: IntVector2D):
      return -turn_dir_left(dir)
  
    def create_corridors(start_direction: IntVector2D):
      count = random.randint(CORRIDOR_BRANCHE_COUNT[0], CORRIDOR_BRANCHE_COUNT[1])
      direction = start_direction
      corridors = []
      for _ in range(count):
        lenght = random.randint(CORRIDOR_LENGHT[0], CORRIDOR_LENGHT[1])
        corridors.append(direction * lenght)
        direction = turn_dir_right(direction) if random.randint(0, 1) == 0 else turn_dir_left(direction)
      return corridors
    
    def create_corridor_branch(position: IntVector2D, directions: list[IntVector2D]):
      nonlocal branch_count
      #print(branch_count)
      #res = []
      next_points = []
      for dir in directions:
        branch_count -= 1
        if branch_count <= 0:
          break
        corridors = create_corridors(dir)
        next_point = sum(corridors, IntVector2D())
        next_point += position
        #next_point = sum(corridors) + position
        if abs(next_point.x) > map_size.x or abs(next_point.y) > map_size.y:
          continue

        last_corridor = corridors[-1]
        last_direction = IntVector2D(sign(last_corridor.x), sign(last_corridor.y))
        next_points.append((next_point, last_direction, corridors))

        #next_btanches.append((position, corridors))

      res = []
      for next_point in next_points:
        res.append((position, next_point[2]))
        if branch_count < 0:
          continue
        pos = next_point[0]
        last_direction = next_point[1]
        next_directions = []
        if random.randint(1, 5) == 1:
          next_directions.append(turn_dir_left(last_direction))
        next_directions.append(last_direction)
        if random.randint(1, 5) == 1:
          next_directions.append(turn_dir_right(last_direction))

        res.extend(create_corridor_branch(pos, next_directions))
      return res

    start_pos = map_size / 2
    start_directions = [
      IntVector2D(1, 0),
      IntVector2D(0, 1),
      IntVector2D(-1, 0),
      IntVector2D(0, -1),
    ]

    corridor_branches = create_corridor_branch(start_pos, start_directions)
    print(f"corridor_branches = {len(corridor_branches)}")

    for corridor_branch in corridor_branches:
      point = corridor_branch[0]
      corridors = corridor_branch[1]
      for corridor in corridors:
        #ZERO_VECTOR
        c = corridor
        c_dir = create_dir_vector(c)
        while c != ZERO_VECTOR:
          map_data[point.x][point.y] = " "
          c -= c_dir

      for m in map_data:
        s = "".join(m)
        print(s)
      return map_data

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
    wall = TileType.WALL.value | ord('#')
    for x in range(width):
      for y in range(height):
        value = maze_data[x][y]
        tile = space if value == " " else wall
        self.map_schema.set_field(map_component, ["tiles", x, y], tile)

  def create_map_2(self, width, height):
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
        t = "*" if random.randint(1, 100) <= 1 else " "
        self.map_schema.set_field(map_component, ["tiles", x, y], ord(t))

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
    
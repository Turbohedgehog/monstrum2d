import random

from enum import Enum

from Core import SystemBase
from Core import IntVector2D

BORDER_SIZE = 1

MAP_SIZE = IntVector2D(300, 150)
#MAP_SIZE = IntVector2D(100, 100)

class TileType(Enum):
  GROUND = 0
  WALL = 1 << 8

KEY_COUNT = 4

class MapGenerator(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.gameplay_ecs = self.holder.get_or_create_ecs("gameplay")

    try:
      map_size = MAP_SIZE
      spawn_pos = map_size / 2
      maze, start_points = MapGenerator.create_maze(map_size, IntVector2D(8, 4), IntVector2D(20, 10))
      self.create_keys(start_points, KEY_COUNT)      
      self.create_exit(spawn_pos)
      MapGenerator.cleanup_maze(maze)
      self.create_map(maze)
      self.create_surviver(spawn_pos)
    except Exception as ex:
      print(ex)
      raise

  def create_surviver(self, spawn_pos: IntVector2D):
    surviver = self.gameplay_ecs.create_entity(["surviver", "coordinate"])
    coordinate_component = surviver.get_component("coordinate")
    coordinate_component.set_field("x", spawn_pos.x)
    coordinate_component.set_field("y", spawn_pos.y)

  def create_exit(self, position: IntVector2D):
    exit = self.gameplay_ecs.create_entity(["exit", "coordinate"])
    coordinate_component = exit.get_component("coordinate")
    coordinate_component.set_field("x", position.x)
    coordinate_component.set_field("y", position.y)

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
    MapGenerator.iterate_maze(start_pos, map_size, map_data, min_room_size, max_room_size)
    for _ in range(30):
      start_pos.x = random.randint(BORDER_SIZE, map_size.x - 1 - BORDER_SIZE)
      start_pos.y = random.randint(BORDER_SIZE, map_size.y - 1 - BORDER_SIZE)
      start_points.append(IntVector2D(start_pos.x, start_pos.y))
      MapGenerator.iterate_maze(start_pos, map_size, map_data, min_room_size, max_room_size)

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
    map_component.set_field("width", width)
    map_component.set_field("height", height)
    map_data = map_component.get_field("tiles")
    map_data.resize(width)

    for i in range(width):
      map_col = map_component.get_field(["tiles", i])
      map_col.resize(height)

    space = ord(" ")
    for x in range(width):
      for y in range(height):
        value = maze_data[x][y]
        tile = space if value == " " else TileType.WALL.value | ord(value)
        map_component.set_field(["tiles", x, y], tile)

  def create_keys(self, start_points, count):
    count = min(count, len(start_points))
    if count == 0:
      return
    
    key_points = random.sample(start_points, count)
    for i, key_point in enumerate(key_points):
      key_entity = self.gameplay_ecs.create_entity(["key", "coordinate"])

      coordinate_component = key_entity.get_component("coordinate")
      coordinate_component.set_field("x", key_point.x)
      coordinate_component.set_field("y", key_point.y)

      key_component = key_entity.get_component("key")
      key_component.set_field("id", i)
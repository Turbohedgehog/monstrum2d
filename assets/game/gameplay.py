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
KEY_COUNT = 4

BRUTE_MOVE_TIME = [1, 0.8, 0.5, 0.3]

BORDER_SIZE = 1

MAP_SIZE = IntVector2D(300, 150)
#MAP_SIZE = IntVector2D(100, 100)

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

    self.gameplay_state_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])
    gameplay_state_entity = self.gameplay_ecs.create_entity(["gameplay_state"])
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    gameplay_state.set_field("screen_id", screen_id)
    self.gameplay_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate"])
    self.brute_filter = self.gameplay_ecs.get_or_create_filter(["brute", "coordinate"])
    self.key_filter = self.gameplay_ecs.get_or_create_filter(["key", "coordinate"])
    self.exit_filter = self.gameplay_ecs.get_or_create_filter(["exit", "coordinate"])

  def update(self, delta):
    super().update(delta)

    try:
      self.update_input(delta)
    except Exception as ex:
      print(ex)
      raise

  def update_input(self, delta):
    gameplay_state_entity = next((gameplay for gameplay in self.gameplay_filter), None)
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    state = gameplay_state.get_field("state")
    if state == 1:
      try:
        self.update_brute(delta)
      except Exception as ex:
        print(ex)
        raise
    key_pressed = self.gameplay_screen.get_key_pressed()
    if key_pressed is not None:
      if key_pressed == 27:
        self.holder.shutdown()
        return
      
      if state == 0:
        gameplay_state.set_field("state", 1)
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
      key_down_timer = surviver_component.get_field("key_down_timer")
      move_direction = surviver_component.get_field("move_direction")

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

      surviver_component.set_field("move_direction", move_direction)
      surviver_component.set_field("key_down_timer", key_down_timer)

      if key_down_timer >= MOVE_TIME:
        key_down_timer -= MOVE_TIME
        surviver_component.set_field("key_down_timer", key_down_timer)
        coordinate_component = surviver_enity.get_component("coordinate")
        x = coordinate_component.get_field("x")
        y = coordinate_component.get_field("y")

        map_component = map_entity.get_component("map")
        width = map_component.get_field("width")
        height = map_component.get_field("height")
        surviver_component.set_field("key_down_timer", key_down_timer)

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
        tile = map_component.get_field(["tiles", x, y])
        if tile & TileType.WALL.value != 0:
          return
        
        coordinate_component.set_field("x", x)
        coordinate_component.set_field("y", y)
        
        surviver_pos = IntVector2D(x, y)

        self.check_key_pickup(surviver_pos, surviver_component)
        self.check_exit(surviver_pos, surviver_component)

    except Exception as ex:
      print(f"~ {ex}")
      raise
  
  def check_key_pickup(self, surviver_pos: IntVector2D, surviver_component):
    for key_entity in self.key_filter:
      key_coordinate_component = key_entity.get_component("coordinate")
      key_x = key_coordinate_component.get_field("x")
      key_y = key_coordinate_component.get_field("y")

      if key_x == surviver_pos.x and key_y == surviver_pos.y:
        self.gameplay_ecs.remove_entity(key_entity.get_id())
        collected_keys = surviver_component.get_field("collected_keys")
        collected_keys += 1
        surviver_component.set_field("collected_keys", collected_keys)
        if collected_keys >= KEY_COUNT:
          for exit_entity in self.exit_filter:
            exit_component = exit_entity.get_component("exit")
            exit_component.set_field("state", 1)

        self.upgrade_brute(surviver_component)
        break

  def check_exit(self, surviver_pos: IntVector2D, surviver_component):
    collected_keys = surviver_component.get_field("collected_keys")
    if collected_keys < KEY_COUNT:
      return
    
    for exit_entity in self.exit_filter:
      exit_coordinate = exit_entity.get_component("coordinate")
      exit_x = exit_coordinate.get_field("x")
      exit_y = exit_coordinate.get_field("y")
      if surviver_pos.x == exit_x and surviver_pos.y == exit_y:
        for gameplay_state_entity in self.gameplay_filter:
          gameplay_state = gameplay_state_entity.get_component("gameplay_state")
          # Good ending
          gameplay_state.set_field("state", 2)
    
  def upgrade_brute(self, surviver_component):
    collected_keys = surviver_component.get_field("collected_keys")
    if collected_keys == 1:
      try:
        self.spawn_brute()
      except Exception as ex:
        print(ex)
        raise
    
    for brute in self.brute_filter:
      brute_component = brute.get_component("brute")
      brute_component.set_field("brute_state", collected_keys - 1)

  def update_brute(self, delta_time):
    map_entity = next((m for m in self.map_filter), None) 
    if map_entity is None:
      return
    
    map_component = map_entity.get_component("map")

    for brute in self.brute_filter:
      self.move_brute(brute, map_component, delta_time)

  def check_brute_attack(self, brute, surviver):
    brute_coordinate = brute.get_component("coordinate")
    surviver_coordinate = surviver.get_component("coordinate")

    if brute_coordinate.get_field("x") != surviver_coordinate.get_field("x"):
      return
    if brute_coordinate.get_field("y") != surviver_coordinate.get_field("y"):
      return
    
    for gameplay in self.gameplay_state_filter:
      gameplay_state = gameplay.get_component("gameplay_state")
      gameplay_state.set_field("state", 3)
    
    return True

  def choose_brute_next_direction(self, brute, surviver):
    brute_component = brute.get_component("brute")
    brute_state = brute_component.get_field("brute_state")

    move_dir_x = 0
    move_dir_y = 0
    if brute_state == 0:
      if random.randint(0, 1) == 0:
        move_dir_x = random.randint(-5, 5)
      else:
        move_dir_y = random.randint(-5, 5)
      return move_dir_x, move_dir_y
    
    brute_coordinate = brute.get_component("coordinate")
    brute_x = brute_coordinate.get_field("x")
    brute_y = brute_coordinate.get_field("y")

    brute_xy = IntVector2D(brute_x, brute_y)
  
    surviver_coordinate = surviver.get_component("coordinate")
    surviver_x = surviver_coordinate.get_field("x")
    surviver_y = surviver_coordinate.get_field("y")
    surviver_xy = IntVector2D(surviver_x, surviver_y)

    diff = surviver_xy - brute_xy
    diff_sing = IntVector2D(sign(diff.x), sign(diff.y))

    if brute_state == 1:
      if random.randint(0, 1) == 0:
        shift_x = diff_sing.x * 3
        move_dir_x = random.randint(-2, 2)
        move_dir_x += shift_x
      else:
        shift_y = diff_sing.y * 3
        move_dir_y = random.randint(-2, 2)
        move_dir_y += shift_y
      return move_dir_x, move_dir_y

    if brute_state == 2:
      abs_x = abs(diff_sing.x)
      abs_y = abs(diff_sing.y)
      if abs_x > abs_y:
        return diff_sing.x, 0
      elif diff_sing.x < diff_sing.y:
        return 0, diff_sing.y
      else:
        return (diff_sing.x, 0) if random.randint(0, 1) == 0 else (0, diff_sing.y)
      
    if brute_state == 3:
      return diff_sing.x, diff_sing.y
    
    raise "Unknown brute state"

  def move_brute(self, brute, map_component, delta_time):
    brute_component = brute.get_component("brute")
    brute_state = brute_component.get_field("brute_state")
    coordinate = brute.get_component("coordinate")
    x = coordinate.get_field("x")
    y = coordinate.get_field("y")
    next_x = x
    next_y = y

    move_timer = brute_component.get_field("move_timer")
    move_timer += delta_time
    time_to_move = BRUTE_MOVE_TIME[brute_state]
    if move_timer >= time_to_move:
      surviver = next((m for m in self.surviver_filter), None) 
      move_timer -= time_to_move
      move_dir_x = brute_component.get_field("move_dir_x")
      move_dir_y = brute_component.get_field("move_dir_y")
      if move_dir_x != 0:
        sign_x = sign(move_dir_x)
        next_x += sign_x
        move_dir_x -= sign_x

      if move_dir_y != 0:
        sign_y = sign(move_dir_y)
        next_y += sign_y
        move_dir_y -= sign_y

      width = map_component.get_field("width")
      height = map_component.get_field("height")
      
      if next_x < BORDER_SIZE or next_x >= width - BORDER_SIZE or next_y < BORDER_SIZE or next_y >= height - BORDER_SIZE:
        # Stay inside the map
        move_dir_x = 0
        move_dir_y = 0
      elif map_component.get_field(["tiles", next_x, next_y]) & TileType.WALL.value == 0:
        # Check if next tile is free
        coordinate.set_field("x", next_x)
        coordinate.set_field("y", next_y)
      elif brute_state < 3:
        # If next tile is wall and Brute state less than 3 then reset move direction
        move_dir_x = 0
        move_dir_y = 0
      else:
        # If next tile is wall and Brule level more equal than 3 then break the wall
        coordinate.set_field("x", next_x)
        coordinate.set_field("y", next_y)
        map_component.set_field(["tiles", next_x, next_y], TileType.GROUND.value | ord(" "))
      
      if move_dir_x == 0 and move_dir_y == 0:
        move_dir_x, move_dir_y = self.choose_brute_next_direction(brute, surviver)
        
      brute_component.set_field("move_dir_x", move_dir_x)
      brute_component.set_field("move_dir_y", move_dir_y)

      self.check_brute_attack(brute, surviver)

    brute_component.set_field("move_timer", move_timer)


  def spawn_brute(self):
    surviver_entity = next((s for s in self.surviver_filter), None) 
    if surviver_entity is None:
      return
    
    surviver_coordinate = surviver_entity.get_component("coordinate")
    s_x = surviver_coordinate.get_field("x")
    s_y = surviver_coordinate.get_field("y")
    surviver_pos = IntVector2D(s_x, s_y)
    
    #squared_length
    coordinates = []
    for key_entity in self.key_filter:
      key_coordinate = key_entity.get_component("coordinate")
      x = key_coordinate.get_field("x")
      y = key_coordinate.get_field("y")
      coordinates.append(IntVector2D(x, y))

    for exit_entity in self.exit_filter:
      exit_coordinate = exit_entity.get_component("coordinate")
      x = exit_coordinate.get_field("x")
      y = exit_coordinate.get_field("y")
      coordinates.append(IntVector2D(x, y))

    max_distance = -1
    far_pos = None

    for coordinate in coordinates:
      c = (surviver_pos - coordinate).squared_length()
      if c > max_distance:
        max_distance = c
        far_pos = coordinate

    if far_pos is None:
      raise "Cannot spawn Brute! Location not found"

    brute_entity = self.gameplay_ecs.create_entity(["brute", "coordinate"])
    brute_component = brute_entity.get_component("brute")
    brute_component.set_field("brute_state", 0)
    brute_coordinate_component = brute_entity.get_component("coordinate")
    brute_coordinate_component.set_field("x", far_pos.x)
    brute_coordinate_component.set_field("y", far_pos.y)

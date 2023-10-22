import random
from enum import Enum

from Core import SystemBase
#from Core import Terminal
from Core import IntVector2D

BORDER_SIZE = 1
BRUTE_MOVE_TIME = [1, 0.8, 0.5, 0.3]

class TileType(Enum):
  GROUND = 0
  WALL = 1 << 8

def sign(value):
  if value > 0:
    return 1
  if value < 0:
    return -1
  return 0

class Brute(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)
    self.holder = system_handler.get_holder()
    self.gameplay_ecs = self.holder.get_or_create_ecs("gameplay")
    self.gameplay_filter = self.gameplay_ecs.get_or_create_filter(["gameplay_state"])
    self.map_ecs = self.holder.get_or_create_ecs("map")
    self.map_filter = self.map_ecs.get_or_create_filter(["map"])
    self.brute_filter = self.gameplay_ecs.get_or_create_filter(["brute", "coordinate"])
    self.surviver_filter = self.gameplay_ecs.get_or_create_filter(["surviver", "coordinate"])


  def update(self, delta):
    super().update(delta)

    gameplay_state_entity = next((gameplay for gameplay in self.gameplay_filter), None)
    gameplay_state = gameplay_state_entity.get_component("gameplay_state")
    state = gameplay_state.get_field("state")
    if state == 1:
      try:
        self.update_brute(delta)
      except Exception as ex:
        print(ex)
        raise

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
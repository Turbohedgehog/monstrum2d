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

BORDER_SIZE = 1

def sign(value):
  if value > 0:
    return 1
  if value < 0:
    return -1
  return 0


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

from Core import SystemBase

class MapGenerator(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()

    self.coordinate_schema = self.holder.get_component_schema("coordinate")
    self.map_tile_schema = self.holder.get_component_schema("map_tile")
    self.bound_schema = self.holder.get_component_schema("bound")

    self.create_map(20, 20)

  def create_map(self, width, height):
    map_ecs = self.holder.get_or_create_ecs("map")
    
    

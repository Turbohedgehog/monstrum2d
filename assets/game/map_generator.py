from Core import SystemBase

class MapGenerator(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()

    self.coordinate_schema = self.holder.get_component_schema("coordinate")
    self.map_tile_schema = self.holder.get_component_schema("map_tile")
    self.bound_schema = self.holder.get_component_schema("bound")
    
    self.system_handler.enable_system_update(self)
    '''
    self.map_schema = self.holder.get_component_schema("map")
    self.map_ecs = self.holder.get_or_create_ecs("map")

    self.create_map(20, 20)
    '''

'''
  def create_map(self, width, height):
    map_entity = self.map_ecs.create_entity(["map"])
    map_component = map_entity.get_component("map")
    self.map_schema.set_field(map_component, "width", width)
    self.map_schema.set_field(map_component, "height", height)
    map_data = self.map_schema.get_field(map_component, "data")
    map_data.resize(width)
    for i in range(width):
      map_col = self.map_schema.get_field(map_component, ["data", i])
      map_col.resize(height)

    wall = 1 << 8 | ord('#')
    for x in range(width):
      for y in range(height):
        self.map_schema.set_field(map_component, ["data", x, y], wall)
'''
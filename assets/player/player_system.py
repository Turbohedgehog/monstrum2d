from Core import SystemBase

class PlayerSystem(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.player_system_ecs = system_handler.get_or_create_ecs("PlayerSystem")
    self.player_component_schema = system_handler.get_schema("player_component")
    self.player_component_filter = self.player_system_ecs.get_or_register_filter("player_component")
    player_entity = self.player_system_ecs.create_entity("player_component")
    player_component = player_entity.get_component("player_component")
    self.player_component_schema.set_player_name(player_component, "Player")
    

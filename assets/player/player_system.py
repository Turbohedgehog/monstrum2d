from Core import SystemBase

class PlayerSystem(SystemBase):
  def __init__(self, system_handler):
    super().__init__(system_handler)

    self.holder = system_handler.get_holder()
    self.player_system_ecs = self.holder.get_or_create_ecs("PlayerSystem")
    self.player_component_schema = self.holder.get_component_schema("player_component")
    #print(f"{type(self.player_component_schema)}")
    #print(f"self.player_component_schema.schema = {self.player_component_schema.schema}")
    #self.player_component_filter = self.player_system_ecs.get_or_register_filter("player_component")
    player_entity = self.player_system_ecs.create_entity(["player_component"])
    player_component = player_entity.get_component("player_component")
    # access by field name
    player_name = self.player_component_schema.get_field(player_component, "player_name")
    print(f"old player_name = {player_name}")
    # access by list of names
    self.player_component_schema.set_field(player_component, ["player_name"], "New Player name")
    # acess by field index
    new_player_name = self.player_component_schema.get_field(player_component, 0)
    print(f"new player_name = {new_player_name}")    

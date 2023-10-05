#include "ecs/holder.h"

#include <iostream>
#include <stdexcept>

#include <boost/format.hpp>

#include "ecs/component_schema_loader.h"
#include "ecs/component_schema.h"
#include "ecs/ecs.h"

#include "py/application.h"

namespace m2d {

namespace ecs {

Holder::Holder(std::size_t id)
  : id_(id) {}

void Holder::AppendComponentSchema(const std::filesystem::path& schema_path) {
  auto schemas = ComponentSchemaLoader::LoadComponentSchemas(schema_path);
  for (auto& schema : schemas) {
    const auto& component_name = schema->GetName();
    if (component_type_indexer_.left.find(component_name) != component_type_indexer_.left.end()) {
      throw std::runtime_error((boost::format("Componentn chema '%s' duplicated!") % component_name).str());
    }

    component_type_indexer_.insert({component_name, component_type_counter_});
    component_schemas_[component_type_counter_] = schema;
    ++component_type_counter_;
  }
}

void Holder::AppendSystems(const std::filesystem::path& systems_path) {
  if (!py_application_) {
    py_application_ = std::make_shared<py::Application>(shared_from_this());
  }

  py_application_->CollectSystems(systems_path);
}

void Holder::Init() {
  py_application_->InitSystems();
}

void Holder::Update(float delta) {
  py_application_->Update(delta);
}

std::size_t Holder::GetECSCount() const {
  return ecs_couter_;
}

bool Holder::IsActive() const {
  return !shutdown_ && py_application_->IsActive();
}

ComponentSchemaWeakPtr Holder::GetSchema(const std::string& schema_name) const {
  auto schema_id_it = component_type_indexer_.left.find(schema_name);
  if (schema_id_it == component_type_indexer_.left.end()) {
    return ComponentSchemaWeakPtr();
  }

  auto schema_it = component_schemas_.find(schema_id_it->second);
  if (schema_it == component_schemas_.end()) {
    return ComponentSchemaWeakPtr();
  }

  return schema_it->second;
}

void Holder::Shutdown() {
  shutdown_ = true;
  std::cout << "Holder shutdown!\n";
}

ECSWeakPtr Holder::GetOrCreateECS(const std::string& ecs_name) {
  auto it = ecs_names_.left.find(ecs_name);
  if (it == ecs_names_.left.end()) {
    ecs_names_.insert({ecs_name, ecs_couter_});
    auto res = ecs_.emplace(
        ecs_couter_, std::make_shared<ECS>(ecs_name, shared_from_this())
    );
    ++ecs_couter_;

    return res.first->second;
  }

  return ecs_[it->second];
}

std::optional<std::size_t> Holder::GetComponentSchemaIdByName(const std::string& schema_name) const {
  auto it = component_type_indexer_.left.find(schema_name);
  if (it == component_type_indexer_.left.end()) {
    return std::nullopt;
  }

  return it->second;
}

ComponentSchemaWeakPtr Holder::GetComponentSchema(std::size_t schema_id) const {
  auto it = component_schemas_.find(schema_id);
  return it != component_schemas_.end() ? it->second : ComponentSchemaWeakPtr();
}

ComponentSchemaWeakPtr Holder::GetComponentSchema(const std::string& schema_name) const {
  auto id = GetComponentSchemaIdByName(schema_name);

  return id ? GetComponentSchema(id.value()) : ComponentSchemaWeakPtr();
}

} // namespace ecs

}  // namespace m2d

#include "ecs/holder.h"

#include <stdexcept>

#include <boost/format.hpp>

#include "ecs/component_schema_loader.h"
#include "ecs/component_shema.h"

#include "py/application.h"

namespace m2d {

namespace ecs {

Holder::Holder(std::size_t id)
  : id_(id)
  , py_application_(new py::Application) {}

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
  py_application_->CollectSystems(systems_path);
}

void Holder::Init() {

}

void Holder::Update(float delta) {

}

std::size_t Holder::GetECSCount() const {
  return ecs_couter_;
}

} // namespace ecs

}  // namespace m2d

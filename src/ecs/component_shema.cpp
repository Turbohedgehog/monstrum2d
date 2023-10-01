#include "ecs/component_shema.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include <boost/format.hpp>

#include "ecs/component_data.h"
#include "ecs/component.h"

namespace m2d {

namespace ecs {

ComponentField::ComponentField(const std::string& name)
  : name_(name) {}

const std::string& ComponentField::GetName() const {
  return name_;
}

ComponentSchema::ComponentSchema(const std::string& name)
  : name_(name) {}

const std::string& ComponentSchema::GetName() const {
  return name_;
}

void ComponentSchema::AppendField(ComponentFieldPtr field) {
  const auto& field_name = field->GetName();
  if (field_index_map_.left.find(field_name) != field_index_map_.left.end()) {
    throw std::runtime_error(
      (
        boost::format("[%s] Attempt to add a field with a duplicate name '%s'") %
          GetName() %
          field->GetName()
      ).str()
    );
  }

  field_index_map_.insert({field_name, field_counter_});
  fields_[field_counter_] = field;

  ++field_counter_;
}

ComponentPtr ComponentSchema::AllocateComponent(ECSWeakPtr /*ecs*/) const {
  return ComponentPtr();
}

ComponentDataPtr ComponentSchema::CreateComponentData(ECSWeakPtr ecs) const {
  auto struct_data = std::make_shared<StructComponentData>();
  std::transform(
      fields_.begin(),
      fields_.end(),
      std::back_inserter(struct_data->data),
      [](const auto& pair){ return ComponentDataPtr(); }
  );

  return struct_data;
}

}  // namespace ecs

}  // namespace m2d

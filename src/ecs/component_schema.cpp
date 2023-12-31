#include "ecs/component_schema.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include <boost/format.hpp>

#include "ecs/ecs.h"
#include "ecs/component_data.h"
#include "ecs/component.h"

namespace m2d {

namespace ecs {

void ComponentStruct::AppendField(std::string&& name, ComponentFieldPtr field) {
  if (field_index_map_.left.find(name) != field_index_map_.left.end()) {
    throw std::runtime_error(
      (
        boost::format("Attempt to add a field with a duplicate name '%s'") %
          name
      ).str()
    );
  }

  
  field_index_map_.insert({std::move(name), fields_.size()});
  fields_.push_back(field);
}

ComponentDataPtr ComponentStruct::AllocateData(ECSWeakPtr ecs) const {
  auto struct_data = std::make_shared<StructComponentData>();
  std::transform(
      fields_.begin(),
      fields_.end(),
      std::back_inserter(struct_data->data),
      [&ecs](auto& field_info) {
        return field_info->AllocateData(ecs);
      });

  return struct_data;
}

ComponentFiledType ComponentStruct::GetType() const {
  return ComponentFiledType::Struct;
}

ComponentDataPtr ComponentStruct::AccessToComponentData(
    ComponentDataPtr component_data,
    const FieldIndexContainer& indices,
    std::size_t idx) const {
  if (idx == indices.size()) {
    return component_data;
  }

  const StringIndex& index = indices[idx];
  std::optional<std::size_t> field_idx;
  std::visit(
    visitor_overload {
      [&field_idx](std::size_t field_index) { field_idx = field_index; },
      [this, &field_idx](const std::string& field_name) {
        auto it = field_index_map_.left.find(field_name);
        if (it != field_index_map_.left.end()) {
          field_idx = it->second;
        }
      },
    },
    index);

  auto field_idx_value = field_idx.value_or(fields_.size());
  if (field_idx_value >= fields_.size()) {
    return ComponentDataPtr();
  }

  auto struct_data = std::static_pointer_cast<StructComponentData>(component_data);
  auto next_data = struct_data->data[field_idx_value];
  auto next_schema = fields_[field_idx_value];

  return next_schema->AccessToComponentData(next_data, indices, idx + 1);
}

////////////////////////////////////////////////////////////////////////////////////////

ComponentArray::ComponentArray(ComponentFieldPtr content)
    : content_ (content) {}

ComponentDataPtr ComponentArray::AllocateData(ECSWeakPtr ecs) const {
  auto array_data = std::make_shared<ArrayComponentData>();
  array_data->content_field = content_;
  array_data->ecs = ecs;

  return array_data;
}

ComponentFiledType ComponentArray::GetType() const {
  return ComponentFiledType::Array;
}

ComponentDataPtr ComponentArray::AccessToComponentData(
      ComponentDataPtr component_data,
      const FieldIndexContainer& indices,
      std::size_t idx) const {
  if (idx == indices.size()) {
    return component_data;
  }

  const StringIndex& index = indices[idx];
  std::optional<std::size_t> array_idx;
  std::visit(
    visitor_overload {
      [&array_idx](std::size_t field_index) { array_idx = field_index; },
      [](const std::string& /*field_name*/) {}
    },
    index);

  if (!array_idx) {
    return ComponentDataPtr();
  }

  auto array_idx_value = array_idx.value();

  auto array_data = std::static_pointer_cast<ArrayComponentData>(component_data);
  if (array_idx_value >= array_data->data.size()) {
    return ComponentDataPtr();
  }

  auto next_data = array_data->data[array_idx_value];

  return content_->AccessToComponentData(next_data, indices, idx + 1);
}

////////////////////////////////////////////////////////////////////////////////////////

ComponentSchema::ComponentSchema(const std::string& name)
  : name_(name)
  , root_(std::make_shared<ComponentStruct>()) {}

const std::string& ComponentSchema::GetName() const {
  return name_;
}

ComponentStructPtr ComponentSchema::GetRoot() const {
  return root_;
}

ComponentPtr ComponentSchema::AllocateComponent(ECSWeakPtr /*ecs*/) const {
  return ComponentPtr();
}

ComponentDataPtr ComponentSchema::CreateComponentData(ECSWeakPtr ecs) const {
  return root_->AllocateData(ecs);
}

ComponentDataPtr ComponentSchema::AccessToComponentData(
    ComponentPtr component,
    const FieldIndexContainer& indices) const {
  return root_->AccessToComponentData(
    component->GetData(),
    indices,
    0
  );
}

void ComponentSchema::SetLifetime(ComponentLifetime lifetime) {
  lifetime_ = lifetime;
}

}  // namespace ecs

}  // namespace m2d

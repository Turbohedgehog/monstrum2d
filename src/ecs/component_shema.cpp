#include "ecs/component_shema.h"

#include <fstream>
#include <stdexcept>

#include <boost/format.hpp>

//#include "ecs/component_data.h"
#include "ecs/component.h"

namespace m2d {

namespace ecs {

ComponentFixedStringField::ComponentFixedStringField(
    const std::string& name,
    std::size_t size,
    const std::string& default_value)
  : ComponentField(name)
  , size_(size)
  , default_value_(default_value.substr(0, size)) {
}

std::size_t ComponentFixedStringField::GetSize() const {
  return size_;
}


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
  if (field_index_map_.left.find(field_name) == field_index_map_.left.end()) {
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

  data_size_ += field->GetSize();
  ++field_counter_;
}

std::size_t ComponentSchema::GetDataSize() const {
  return data_size_;
}

ComponentPtr ComponentSchema::AllocateComponent(ECSWeakPtr ecs) const {
  return ComponentPtr();
}

}  // namespace ecs

}  // namespace m2d

#include "ecs/component_shema.h"

#include <fstream>
#include <stdexcept>

#include "yaml-cpp/yaml.h"

namespace m2d {

namespace ecs {

/*
bool ComponentSchema::Load(const std::filesystem::path& schema_path) {
  YAML::Node doc = YAML::LoadFile(schema_path.generic_string());
  if (!doc.IsSequence()) {
    throw std::invalid_argument("Wrong document format");
  }

  return false;
}
*/

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

}

}  // namespace ecs

}  // namespace m2d

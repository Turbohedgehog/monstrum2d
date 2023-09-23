#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include <boost/bimap.hpp>

//#include "ecs/asset.h"

namespace m2d {

namespace ecs {
  
class ComponentField {
 public:
  ComponentField(const std::string& name);

  const std::string& GetName() const;
  virtual std::size_t GetSize() const = 0;

 private:
  std::string name_;
};

template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
class ComponentPrimitiveField : public ComponentField {
 public:
  ComponentPrimitiveField(const std::string& name, const T& default_value = static_cast<T>(0))
    : ComponentField(name)
    , default_value_(default_value) {
  }

  std::size_t GetSize() const override {
    return sizeof(T);
  }

  const T& GetDefaultValue() const {
    return default_value_;
  }

 private:
  T default_value_;
};

using ComponentIntField = ComponentPrimitiveField<int>;
using ComponentDoubleField = ComponentPrimitiveField<double>;

//class ComponentIntField : public ComponentPrimitiveField<int> {};

//class ComponentDoubleField : public ComponentPrimitiveField<double> {};

//class ComponentFixedStringField : public ComponentField {};

//class ComponentFixedArrayString : public ComponentField {};

using ComponentFieldPtr = std::shared_ptr<ComponentField>;

class ComponentSchema {
 public:
  ComponentSchema(const std::string& name);
  const std::string& GetName() const;

  void AppendField(ComponentFieldPtr field);

  template <typename T, typename... Ts, std::enable_if_t<std::is_base_of_v<ComponentField, T>, bool> = true>
  void AppendField(Ts&&... args) {
    AppendField(std::make_shared<T>(std::forward<Ts>(args)...));
  }

 private:
  std::string name_;

  boost::bimap<std::string, std::size_t> field_index_map_;
  std::size_t field_counter_ = 0;
  std::map<std::size_t, ComponentFieldPtr> fields_;
};

}  // namespace ecs

}  // namespace m2d

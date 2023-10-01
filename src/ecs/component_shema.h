#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include <boost/bimap.hpp>

#include "common/common_types.h"

#include "ecs/component_data.h"

namespace m2d {

namespace ecs {
  
class ComponentField {
 public:
  ComponentField(const std::string& name);

  const std::string& GetName() const;
  virtual std::size_t GetSize() const = 0;

  virtual ComponentDataPtr AllocateData(ECSWeakPtr ecs) const = 0;

 private:
  std::string name_;
};

//template <typename T, typename = std::enable_if_t<std::is_trivial_v<T>>>
template <typename T>
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

  ComponentDataPtr AllocateData(ECSWeakPtr /*ecs*/) const override {
    auto data = std::make_shared<ComponentPrimitiveData<T>>();
    data->data = default_value_;

    return data;
  }

 private:
  T default_value_;
};

using ComponentIntField = ComponentPrimitiveField<int>;
using ComponentDoubleField = ComponentPrimitiveField<double>;
class ComponentFixedStringField : public ComponentField {
 public:
  ComponentFixedStringField(const std::string& name, std::size_t size, const std::string& default_value = "");
  std::size_t GetSize() const override;

  ComponentDataPtr AllocateData(ECSWeakPtr /*ecs*/) const override {
    return ComponentDataPtr();
  }

 private:
  std::size_t size_ = 0;
  std::string default_value_;

};

// class ComponentFixedArray : public ComponentField {};
class ComponentStruct : public ComponentField {

};

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

  ComponentPtr AllocateComponent(ECSWeakPtr ecs) const;
  std::size_t GetDataSize() const;

  ComponentDataPtr CreateComponentData(ECSWeakPtr ecs) const;

 private:
  std::string name_;

  boost::bimap<std::string, std::size_t> field_index_map_;
  std::size_t field_counter_ = 0;
  std::map<std::size_t, ComponentFieldPtr> fields_;
  std::size_t data_size_ = 0;
};

}  // namespace ecs

}  // namespace m2d

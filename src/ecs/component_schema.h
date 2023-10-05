#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <type_traits>

#include <boost/bimap.hpp>

#include "common/common_types.h"
#include "ecs/component_field_type.h"
#include "common/default_value.h"

#include "ecs/component_data.h"

namespace m2d {

namespace ecs {

using FieldIndexContainer = std::vector<StringIndex>;

class ComponentField {
 public:
  virtual ComponentDataPtr AllocateData(ECSWeakPtr ecs) const = 0;
  virtual ComponentFiledType GetType() const = 0;
  virtual ComponentDataPtr AccessToComponentData(
      ComponentDataPtr component_data,
      const FieldIndexContainer& indices,
      std::size_t idx) const = 0;
};

template <typename T>
class ComponentPrimitiveField : public ComponentField {
 public:
  ComponentPrimitiveField(const T& default_value = DefaultValue<T>::value)
    : default_value_(default_value) {
      volatile int zz = 0;
    }

  const T& GetDefaultValue() const {
    return default_value_;
  }

  ComponentDataPtr AllocateData(ECSWeakPtr /*ecs*/) const override {
    auto data = std::make_shared<ComponentPrimitiveData<T>>();
    data->data = default_value_;

    return data;
  }

  ComponentFiledType GetType() const override {
    return ComponentTypeV<T>;
  }

  ComponentDataPtr AccessToComponentData(
      ComponentDataPtr component_data,
      const FieldIndexContainer& /*indices*/,
      std::size_t /*idx*/) const override {
    return component_data;
  }

 private:
  T default_value_;
};

using ComponentFieldPtr = std::shared_ptr<ComponentField>;

// class ComponentArray : public ComponentField {};

class ComponentStruct : public ComponentField {
 public:
  void AppendField(std::string&& name, ComponentFieldPtr field);

  template <typename T, typename... Ts, std::enable_if_t<std::is_base_of_v<ComponentField, T>, bool> = true>
  void AppendField(std::string&& name, Ts&&... args) {
    AppendField(std::move(name), std::make_shared<T>(std::forward<Ts>(args)...));
  }

  ComponentDataPtr AllocateData(ECSWeakPtr ecs) const override;

  ComponentFiledType GetType() const override;

  ComponentDataPtr AccessToComponentData(
      ComponentDataPtr component_data,
      const FieldIndexContainer& indices,
      std::size_t idx) const override;

 private:
  boost::bimap<std::string, std::size_t> field_index_map_;
  //std::size_t field_counter_ = 0;
  std::vector<ComponentFieldPtr>  fields_;
  //std::map<std::size_t, ComponentFieldPtr> fields_;
};

using ComponentStructPtr = std::shared_ptr<ComponentStruct>;

class ComponentSchema {
 public:
  ComponentSchema(const std::string& name);
  const std::string& GetName() const;
  ComponentPtr AllocateComponent(ECSWeakPtr ecs) const;
  ComponentDataPtr CreateComponentData(ECSWeakPtr ecs) const;
  ComponentStructPtr GetRoot() const;
  ComponentDataPtr AccessToComponentData(
      ComponentPtr component,
      const FieldIndexContainer& indices) const;

 private:
  std::string name_;

  ComponentStructPtr root_;
};

}  // namespace ecs

}  // namespace m2d

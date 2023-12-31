#pragma once

#include <variant>
#include <memory>
#include <string>
#include <bitset>
#include <map>

template <typename... Ts> struct visitor_overload : Ts... { using Ts::operator()...; };
template <typename... Ts> visitor_overload(Ts...) -> visitor_overload<Ts...>;

namespace m2d {

namespace ecs {

using StringIndex = std::variant<std::string, std::size_t>;
using ComponentLifetime = std::variant<double, int>;

static const std::size_t kMaxComponentIndex = 256;
using ComponentBitmask = std::bitset<kMaxComponentIndex>;

class Holder;
using HolderPtr = std::shared_ptr<Holder>;
using HolderWeakPtr = std::weak_ptr<Holder>;

class ECS;
using ECSPtr = std::shared_ptr<ECS>;
using ECSWeakPtr = std::weak_ptr<ECS>;

class Entity;
using EntityPtr = std::shared_ptr<Entity>;
using EntityWeakPtr = std::weak_ptr<Entity>;

using EntityMap = std::map<std::size_t, EntityWeakPtr>;

class System;
using SystemPtr = std::shared_ptr<System>;

class Filter;
using FilterPtr = std::shared_ptr<Filter>;
using FilterWeakPtr = std::weak_ptr<Filter>;

class Component;
using ComponentPtr = std::shared_ptr<Component>;
using ComponentWeakPtr = std::weak_ptr<Component>;

struct ComponentDataBase;
using ComponentDataPtr = std::shared_ptr<ComponentDataBase>;

class ComponentSchema;
using ComponentSchemaPtr = std::shared_ptr<ComponentSchema>;
using ComponentSchemaWeakPtr = std::weak_ptr<ComponentSchema>;

class Pool;
using PoolPtr = std::shared_ptr<Pool>;
using PoolWeakPtr = std::weak_ptr<Pool>;

class ArrayComponentData;
using ArrayComponentDataWeakPtr = std::weak_ptr<ArrayComponentData>;

class ComponentField;
using ComponentFieldPtr = std::shared_ptr<ComponentField>;

}  // namespace ecs

namespace py {

class Application;
using ApplicationPtr = std::shared_ptr<Application>;
using ApplicationWeakPtr = std::weak_ptr<Application>;

}  // namespace py

}  // namespace m2d


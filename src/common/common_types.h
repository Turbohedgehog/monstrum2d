#pragma once

#include <variant>
#include <memory>
#include <string>

template <typename... Ts> struct visitor_overload : Ts... { using Ts::operator()...; };
template <typename... Ts> visitor_overload(Ts...) -> visitor_overload<Ts...>;

namespace m2d {

namespace ecs {

using StringIndex = std::variant<std::string, std::size_t>;
using ComponentLifetime = std::variant<double, int>;

class Holder;
using HolderPtr = std::shared_ptr<Holder>;
using HolderWeakPtr = std::weak_ptr<Holder>;

class ECS;
using ECSPtr = std::shared_ptr<ECS>;
using ECSWeakPtr = std::weak_ptr<ECS>;

class Entity;
using EntityPtr = std::shared_ptr<Entity>;
using EntityWeakPtr = std::weak_ptr<Entity>;

class System;
using SystemPtr = std::shared_ptr<System>;

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

}  // namespace ecs

namespace py {

class Application;
using ApplicationPtr = std::shared_ptr<Application>;
using ApplicationWeakPtr = std::weak_ptr<Application>;

}  // namespace py

}  // namespace m2d

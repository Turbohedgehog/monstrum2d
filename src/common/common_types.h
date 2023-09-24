#pragma once

#include <memory>

namespace m2d {

namespace ecs {

class Holder;
using HolderPtr = std::shared_ptr<Holder>;
using HolderWeakPtr = std::weak_ptr<Holder>;

class ECS;
using ECSPtr = std::shared_ptr<ECS>;
using ECSWeakPtr = std::weak_ptr<ECS>;

class Entity;
using EntityPtr = std::shared_ptr<Entity>;
using EntityWeakPtr = std::weak_ptr<Entity>;

class Component;
using ComponentPtr = std::shared_ptr<Component>;
using ComponentWeakPtr = std::weak_ptr<Component>;

//using ComponentDataPtr = std::shared_ptr<char[]>;
//using ComponentDataWeakPtr = std::weak_ptr<char[]>;

class ComponentSchema;
using ComponentSchemaPtr = std::shared_ptr<ComponentSchema>;

class Pool;
using PoolPtr = std::shared_ptr<Pool>;
using PoolWeakPtr = std::weak_ptr<Pool>;

}  // namespace ecs

}  // namespace m2d
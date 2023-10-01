#pragma once

#include <map>
#include <memory>
#include <string>
#include <filesystem>

#include <common/common_types.h>
#include <boost/bimap.hpp>

namespace m2d {

namespace ecs {
  
class Holder : public std::enable_shared_from_this<Holder> {
 public:
  Holder(std::size_t id);

  void AppendComponentSchema(const std::filesystem::path& schema_path);
  void AppendSystems(const std::filesystem::path& systems_path);
  void Init();
  void Update(float delta);
  bool IsActive() const;
  std::size_t GetECSCount() const;
  ComponentSchemaWeakPtr GetSchema(const std::string& schema_name) const;

  void Shutdown();

 private:
  std::size_t id_;

  bool shutdown_ = false;

  std::size_t component_type_counter_ = 0;
  boost::bimap<std::string, std::size_t> component_type_indexer_;
  std::map<std::size_t, ComponentSchemaPtr> component_schemas_;

  std::size_t ecs_couter_ = 0;
  std::map<std::size_t, ECSPtr> ecs_;

  py::ApplicationPtr py_application_;
};

}  // namespace ecs

}  // namespace m2d

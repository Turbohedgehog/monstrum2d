#pragma once

#include <map>
#include <memory>
#include <string>
#include <filesystem>

#include <common/common_types.h>
#include <boost/bimap.hpp>

namespace m2d {

namespace ecs {
  
class Holder : std::enable_shared_from_this<Holder> {
 public:
  Holder(std::size_t id);

  void AppendComponentSchema(const std::filesystem::path& schema_path);
  void Update(float delta);
  std::size_t GetSystemCount() const;

 private:
  std::size_t id_;

  std::size_t component_type_counter_ = 0;
  boost::bimap<std::string, std::size_t> component_type_indexer_;

  std::size_t system_count_ = 0;
};

}  // namespace ecs

}  // namespace m2d

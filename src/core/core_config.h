#pragma once

#include <string>
#include <vector>

namespace m2d {

class CoreConfig {
 public:
  void Parse(int argc, char* argv[]);
  const std::string& GetConfigFilename() const;
  void LoadConfig();
  const std::string& GetName() const;
  int GetMajorVersion() const;
  int GetMinorVersion() const;
  const std::vector<std::string>& GetECSComponentSchemaPaths() const;

 private:
  std::string config_filename_;

  std::string name_;
  int major_version_ = 0;
  int minor_version_ = 0;
  std::vector<std::string> ecs_component_schema_paths_;
};
  
}  // namespace m2d

#include "core/core_config.h"

#include <iostream>

#include <boost/program_options.hpp>

#include "yaml-cpp/yaml.h"

namespace m2d {

void CoreConfig::Parse(int argc, char* argv[]) {
  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Show help")
    ("config-file,c", po::value<std::string>(&config_filename_)->default_value("config.yaml"), "Start config file")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
  }
}

const std::string& CoreConfig::GetConfigFilename() const {
  return config_filename_;
}

const std::string& CoreConfig::GetName() const {
  return name_;
}

void CoreConfig::LoadConfig() {
  YAML::Node config_doc = YAML::LoadFile(config_filename_);
  if (!config_doc.IsMap()) {
    throw std::runtime_error("Wrong config format");
  }

  auto name_node = config_doc["name"];
  if (!name_node) {
    throw std::runtime_error("name is not set!");
  }
  name_ = name_node.as<std::string>();

  auto version_node = config_doc["version"];
  if (version_node) {
    auto major = version_node["major"];
    if (major) {
      major_version_ = major.as<int>();
    }
    auto minor = version_node["minor"];
    if (minor) {
      minor_version_ = minor.as<int>();
    }
  }

  auto ecs = config_doc["ecs"];
  if (ecs) {
    auto component = ecs["component"];
    if (component) {
      if (!component.IsSequence()) {
        throw std::runtime_error("ECS component section must be a sequence!");
      }

      for (auto val : component) {
        auto schemas = val["schemas"];
        if (schemas) {
          ecs_component_schema_paths_.emplace_back(schemas.as<std::string>());
        }
      }
    }
  }

  auto fps = config_doc["fps"];
  if (fps) {
    if (!fps.IsScalar()) {
      throw std::runtime_error("FPS value must be scalar!");
    }
    fps_ = fps.as<double>();
    if (fps_ <= 0.) {
      throw std::runtime_error("FPS value must be greater tnah zero!");
    }
  }
}

int CoreConfig::GetMajorVersion() const {
  return major_version_;
}

int CoreConfig::GetMinorVersion() const {
  return minor_version_;
}

const std::vector<std::string>& CoreConfig::GetECSComponentSchemaPaths() const {
  return ecs_component_schema_paths_;
}

double CoreConfig::GetFPS() const {
  return fps_;
}

}  // namespace m2d
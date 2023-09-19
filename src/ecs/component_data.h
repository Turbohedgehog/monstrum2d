#pragma once

#include <vector>

namespace m2d {

namespace ecs {
  
class ComponentData {
 public:
  ComponentData(std::size_t size);
  ComponentData(const ComponentData& component_data);

  std::vector<char>& GetData();
  const std::vector<char>& GetData() const;

 private:
  std::vector<char> data_;
};


}  // namespace ecs

}  // namespace m2d

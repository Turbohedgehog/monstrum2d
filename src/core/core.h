#pragma once

namespace m2d {

class Core {
 public:
  //Core() = delete;
  //Core(const Core&) = delete;
  //Core(Core&&) = delete;

  static Core& GetInstance();

  int Run(int *argc, char* argv[]);
};

}  // namespace m2d

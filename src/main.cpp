#include "core/core.h"

int main(int argc, char* argv[]) {
  return m2d::Core::GetInstance().Run(argc, argv);
}
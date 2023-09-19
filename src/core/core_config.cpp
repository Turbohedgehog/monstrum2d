#include "core/core_config.h"

#include <iostream>

#include <boost/program_options.hpp>

namespace m2d {

void CoreConfig::Parse(int argc, char* argv[]) {
  namespace po = boost::program_options;
  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Show help")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << desc << "\n";
  }
}

}  // namespace m2d
#include "py_api/application.h"

#include <iostream>
#include <fstream>

#include "py_api/core_module.h"

namespace m2d {

namespace py {

namespace bp = boost::python;

std::size_t Application::instance_count_ = 0;

Application::Application() {
  if (Application::instance_count_++ == 0) {
    PyImport_AppendInittab("Core", &PyInit_Core);
    Py_Initialize();
  }

  //bp::object sys = bp::import("sys");
  bp::object main = bp::import("__main__");
  global_ = bp::object(main.attr("__dict__"));
  builtins_module_ = global_["__builtins__"];
  inspect_module_ = bp::import("inspect");
  bp::object core_module = bp::import("Core");
  //system_base_class_ = core_module["SystemBase"];
  system_base_class_ = core_module.attr("SystemBase");
}

Application::~Application() {
  if (--Application::instance_count_ == 0) {
    // https://www.boost.org/doc/libs/1_76_0/libs/python/doc/html/tutorial/tutorial/embedding.html#tutorial.embedding.getting_started
    // Py_Finalize();
  }
}

void Application::CollectSystems(const std::filesystem::path& system_path) {
  assert(std::filesystem::exists(system_path));

  std::ifstream file_stream(system_path);
  std::string py_script((std::istreambuf_iterator<char>(file_stream)),
      std::istreambuf_iterator<char>());
  file_stream.close();
  if (py_script.empty()) {
    return;
  }

  try {
    bp::dict local;
    bp::exec(py_script.c_str(), global_, local);
    bp::list items = local.items();
    for (bp::ssize_t i = 0; i < bp::len(items); ++i) {
      bp::tuple key_value = bp::extract<bp::tuple>(items[i]);
      bp::object item_name = key_value[0];
      bp::object item = key_value[1];
      if (!inspect_module_.attr("isclass")(item)) {
        continue;
      }

      if (!builtins_module_.attr("issubclass")(item, system_base_class_)) {
        continue;
      }

      if (item == system_base_class_) {
        continue;
      }

      std::string val = bp::extract<std::string>(item_name);
      std::cout << val << "\n";
    }
  } catch (boost::python::error_already_set& /*ex*/) {
    PyErr_Print();
    throw;
  }
}

}  // namespace py

}  // namespace m2d

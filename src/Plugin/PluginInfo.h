#ifndef PLUGIN_INFO_H
#define PLUGIN_INFO_H

#include "Plugin/PluginSystemInterface.h"
#include <boost/dll.hpp>
#include <memory>
#include <string>

struct PluginInfo {
  std::string name;
  boost::dll::shared_library library;
  std::shared_ptr<PluginSystemInterface> system;
  std::vector<std::string> params_names;
  std::vector<std::type_index> params_types;
  std::vector<void *> params;
};

#endif

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
};

#endif

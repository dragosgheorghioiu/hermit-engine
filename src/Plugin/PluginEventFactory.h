#ifndef PLUGIN_EVENT_FACTORY_H
#define PLUGIN_EVENT_FACTORY_H

#include <string>
#include <unordered_map>
#include <vector>

struct EventFactoryInfo {
	std::string name;
	void *(*createInstance)(...);
	void (*destroyInstance)(void *);

	EventFactoryInfo(std::string name = "", void *(*createInstance)(...) = nullptr,
	                 void (*destroyInstance)(void *) = nullptr)
	    : name(name), createInstance(createInstance), destroyInstance(destroyInstance) {}

	std::string getName() { return name; }
	void *(*getCreateInstance())(...) { return createInstance; }
	void (*getDestroyInstance())(void *) { return destroyInstance; }
};

class PluginEventFactory {
private:
	std::unordered_map<std::string, EventFactoryInfo> events;
	int size;
public:
	PluginEventFactory() = default;
	~PluginEventFactory() = default;

	void loadEvents(const std::string &path);
	void loadEvent(const std::string &path);
	void unloadEvents();
	void unloadEvent(const std::string &name);
};

#endif

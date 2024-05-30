#include "Config.h"

Config& Config::getInstance()
{
	static Config s_cfg{};
	return s_cfg;
}

const size_t Config::getDefaultNumberOfThreads() const
{
	return _default_number_of_threads;
}

const std::string& Config::getDefaultSearchDirectory() const
{
	return _default_search_directory;
}

Config::Config()
{
}

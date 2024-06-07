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

const std::string& Config::getDefaultServerIp() const
{
	return _default_server_ip;
}

const short Config::getDefaultServerPort() const
{
	return _default_server_port;
}

Config::Config()
{
}

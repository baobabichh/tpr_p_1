#pragma once
#include <string>

class Config
{
public:
	static Config& getInstance();
	const size_t getDefaultNumberOfThreads()const;
	const std::string& getDefaultSearchDirectory()const;

	Config(const Config& other) = delete;
	Config(Config&& other) noexcept = delete;
	void operator=(const Config& other) = delete;
	void operator=(Config&& other) = delete;
private:
	Config();

private:
	const size_t _default_number_of_threads{ 8 };
	const std::string _default_search_directory{ "/" };
};
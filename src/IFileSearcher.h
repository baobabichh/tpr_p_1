#pragma once
#include <filesystem>
#include <vector>

class IFileSearcher
{
public:
	virtual void setFileName(const std::filesystem::path& file_name);
	virtual void setNumberOfThreads(const size_t number_of_threads);
	virtual void setStartDirectory(const std::filesystem::path& start_dir);
	virtual void find();
	virtual const std::vector<std::filesystem::path>& getResults()const;
	virtual ~IFileSearcher() = default;
};
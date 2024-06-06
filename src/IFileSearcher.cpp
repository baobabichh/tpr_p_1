#include "IFileSearcher.h"

void IFileSearcher::setFileName(const std::filesystem::path& file_name)
{
}

void IFileSearcher::setNumberOfThreads(const size_t number_of_threads)
{
}

void IFileSearcher::setStartDirectory(const std::filesystem::path& start_dir)
{
}

void IFileSearcher::find()
{
}

const std::vector<std::filesystem::path>& IFileSearcher::getResults() const
{
	static const std::vector<std::filesystem::path> s{};
	return s;
}

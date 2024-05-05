#include "FileSearcher.h"

#include <filesystem>

void FileSearcher::setFileName(const std::string& file_name)
{
	if (file_name.empty())
	{
		return;
	}
	_file_name = file_name;
}

void FileSearcher::setNumberOfThreads(const size_t number_of_threads)
{
	if (number_of_threads == 0)
	{
		return;
	}
	_number_of_threads = number_of_threads;
}

void FileSearcher::setStartDirectory(const std::string& start_dir)
{
	if (start_dir.empty())
	{
		return;
	}
	_start_dir = start_dir;
}

void FileSearcher::find()
{
	_results.clear();
	_should_stop = false;
	_dirs = {};

	if (_file_name.empty() || _start_dir.empty() || _number_of_threads == 0)
	{
		return;
	}

	findImpl();
}

const std::vector<std::string>& FileSearcher::getResults() const
{
	return _results;
}

void FileSearcher::findImpl()
{
	_dirs.push(_start_dir);

	std::vector<std::thread> threads{};
	for (int i = 0; i < _number_of_threads; i++)
	{
		threads.emplace_back(std::thread(&FileSearcher::workerThread, this));
	}

	_new_dir_sem.post();

	int64_t free_threads{ (int64_t)_number_of_threads - 1};
	while (true)
	{
		{
			_thread_finished_sem.wait();
			++free_threads;
		}

		{
			std::unique_lock lk(_dirs_m);
			if (_dirs.empty() && free_threads == _number_of_threads)
			{
				_should_stop = true;
				for (size_t i = 0; i < _number_of_threads; i++)
				{
					_new_dir_sem.post();
				}
				break;
			}
			else if (_dirs.size())
			{
				free_threads--;
				_new_dir_sem.post();
			}
		}
	}

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}
}

void FileSearcher::workerThread()
{
	while (true)
	{
		_new_dir_sem.wait();

		if (_should_stop)
		{
			return;
		}

		std::string dir{};
		{
			std::unique_lock lk(_dirs_m);
			dir = _dirs.front();
			_dirs.pop();
		}

		try
		{
			for (auto const& dir_entry : std::filesystem::directory_iterator{ dir })
			{
				if (dir_entry.is_regular_file())
				{
					std::string fname = dir_entry.path().filename().string();
					if (fname == _file_name)
					{
						std::string path = dir_entry.path().string();
						{
							std::unique_lock lk(_results_m);
							_results.push_back(path);
						}
					}
				}
				else if (dir_entry.is_directory())
				{
					std::string next_dir = dir_entry.path().string();
					{
						std::unique_lock lk(_dirs_m);
						_dirs.push(next_dir);
					}
				}
			}
		}
		catch (...)
		{

		}

		_thread_finished_sem.post();
	}
}

std::vector<std::string> FileSearcher::findFile(const std::string& file_name, const size_t number_of_threads, const std::string& start_dir)
{
	FileSearcher searcher{};
	searcher.setFileName(file_name);
	searcher.setNumberOfThreads(number_of_threads);
	searcher.setStartDirectory(start_dir);
	searcher.find();
	return searcher.getResults();
}

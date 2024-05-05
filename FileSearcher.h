#pragma once

#include "Semaphore.h"
#include <vector>
#include <queue>
#include <thread>

class FileSearcher
{
public:
	void setFileName(const std::string& file_name);
	void setNumberOfThreads(const size_t number_of_threads);
	void setStartDirectory(const std::string& start_dir);
	void find();
	const std::vector<std::string>& getResults()const;

	static std::vector<std::string> findFile(const std::string& file_name, const size_t number_of_threads = 4, const std::string& start_dir = "/");

private:
	void findImpl();
	void workerThread();

private:
	std::string _start_dir{ "/" };
	std::string _file_name{};
	size_t _number_of_threads{ 4 };
	bool _should_stop{ false };

	std::vector<std::string> _results{};
	std::mutex _results_m{};

	std::queue<std::string> _dirs{};
	std::mutex _dirs_m{};

	std::queue<std::string> _new_dirs{};
	std::mutex _new_dirs_m{};

	Semaphore _new_dir_sem{};
	Semaphore _thread_finished_sem{};
};

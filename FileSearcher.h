#pragma once

#include "Semaphore.h"
#include <vector>
#include <queue>
#include <thread>
#include <filesystem>

class FileSearcher
{
public:
	void setFileName(const std::filesystem::path& file_name);
	void setNumberOfThreads(const size_t number_of_threads);
	void setStartDirectory(const std::filesystem::path& start_dir);
	void find();
	const std::vector<std::filesystem::path>& getResults()const;

	static std::vector<std::filesystem::path> findFile(const std::filesystem::path& file_name, const size_t number_of_threads = 4, const std::filesystem::path& start_dir = "/");

private:
	void findImpl();
	void workerThread();

private:
	std::filesystem::path _start_dir{ "/" };
	std::filesystem::path _file_name{};
	size_t _number_of_threads{ 4 };
	bool _should_stop{ false };

	std::vector<std::filesystem::path> _results{};
	std::mutex _results_m{};

	std::queue<std::filesystem::path> _dirs{};
	std::mutex _dirs_m{};

	std::queue<std::filesystem::path> _new_dirs{};
	std::mutex _new_dirs_m{};

	Semaphore _new_dir_sem{};
	Semaphore _thread_finished_sem{};
};

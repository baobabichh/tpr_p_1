#pragma once

#include "Semaphore.h"
#include <vector>
#include <queue>
#include <thread>
#include <filesystem>
#include "IFileSearcher.h"
#include "Config.h"

class FileSearcher : public IFileSearcher
{
public:
	void setFileName(const std::filesystem::path& file_name) override;
	void setNumberOfThreads(const size_t number_of_threads) override;
	void setStartDirectory(const std::filesystem::path& start_dir) override;
	void find() override;
	const std::vector<std::filesystem::path>& getResults()const override;

	static std::vector<std::filesystem::path> findFile(const std::filesystem::path& file_name, const size_t number_of_threads = Config::getInstance().getDefaultNumberOfThreads(), const std::filesystem::path& start_dir = Config::getInstance().getDefaultSearchDirectory());

private:
	void findImpl();
	void workerThread();

private:
	std::filesystem::path _start_dir{ "" };
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

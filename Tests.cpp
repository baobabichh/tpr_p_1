#include "Tests.h"
#include <set>

bool runAllTests()
{
	bool result_var = true;
	#define RUN_TEST(f) {const auto res = f(); if(!res){result_var = false;} std::cout << #f << " " << ((res) ? "OK" : "FAIL") << "\n";}

	RUN_TEST(test_Semaphore);
	RUN_TEST(test_FileSearcher);
	RUN_TEST(test_I_FileSearcher);

	return result_var;
}

bool test_Semaphore()
{
	Semaphore sem{};

	const size_t value = 12;

	for (size_t i = 0; i < value; i++)
	{
		sem.post();
	}

	std::atomic<size_t> counter{};
	const auto f = [&]() {counter++; };

	std::vector<std::thread> threads{};
	for (size_t i = 0; i < value; i++)
	{
		threads.emplace_back(std::thread{ f });
	}

	for (auto& thread : threads)
	{
		if (thread.joinable())
		{
			thread.join();
		}
	}

	return counter == value;
}

bool test_FileSearcher()
{
	{
		auto results = FileSearcher::findFile("New Text Document.txt", 8, "test_dir");
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"test_dir\\1\\2\\New Text Document.txt",
			"test_dir\\1\\4\\New Text Document.txt",
			"test_dir\\1\\2\\3\\New Text Document.txt",
		};

		if (expected != pathes)
		{
			return false;
		}
	}

	{
		auto results = FileSearcher::findFile("1.txt", 8, "test_dir");
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"test_dir\\1.txt",
		};

		if (pathes != expected)
		{
			return false;
		}
	}

	return true;
}

bool test_I_FileSearcher()
{
	{
		std::unique_ptr<IFileSearcher> file_searcher = std::make_unique<FileSearcher>();
		file_searcher->setFileName("New Text Document.txt");
		file_searcher->setNumberOfThreads(8);
		file_searcher->setStartDirectory("test_dir");
		file_searcher->find();
		auto results = file_searcher->getResults();
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"test_dir\\1\\2\\New Text Document.txt",
			"test_dir\\1\\4\\New Text Document.txt",
			"test_dir\\1\\2\\3\\New Text Document.txt",
		};

		if (expected != pathes)
		{
			return false;
		}
	}

	{
		auto results = FileSearcher::findFile("1.txt", 8, "test_dir");
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"test_dir\\1.txt",
		};

		if (pathes != expected)
		{
			return false;
		}
	}

	return true;
}

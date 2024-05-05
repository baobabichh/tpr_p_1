#include "Tests.h"
#include <set>

bool runAllTests()
{
	bool result_var = true;
	#define RUN_TEST(f) {const auto res = f(); if(!res){result_var = false;} std::cout << #f << " " << ((res) ? "OK" : "FAIL") << "\n";}

	RUN_TEST(test_Semaphore);
	RUN_TEST(test_FileSearcher);

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
		auto results = FileSearcher::findFile("New Text Document.txt", 8, "C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir");
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir\\1\\2\\New Text Document.txt",
			"C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir\\1\\4\\New Text Document.txt",
			"C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir\\1\\2\\3\\New Text Document.txt",
		};

		if (pathes != expected)
		{
			return false;
		}
	}

	{
		auto results = FileSearcher::findFile("1.txt", 8, "C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir");
		std::set<std::filesystem::path> pathes{};
		for (const auto& result : results)
		{
			pathes.insert(result);
		}
		const std::set<std::filesystem::path> expected =
		{
			"C:\\Users\\goshan\\source\\repos\\tpr_p_1\\test_dir\\1.txt",
		};

		if (pathes != expected)
		{
			return false;
		}
	}

	return true;
}

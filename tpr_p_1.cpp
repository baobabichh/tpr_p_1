#include <iostream>
#include <string>
#include "FileSearcher.h"

#define RUN_TESTS

#ifdef RUN_TESTS
#include "Tests.h"
int main()
{
	return !runAllTests();
}

#else
int main()
{
	std::string filename{};
	{
		std::cout << "Enter file name: ";
		std::getline(std::cin, filename);
	}

	size_t number_of_threads{};
	{
		std::cout << "Enter number of threads: ";
		while (true)
		{
			std::string buff{};
			std::getline(std::cin, buff);
			try
			{
				number_of_threads = std::stoull(buff);
				break;
			}
			catch (...)
			{
				std::cout << "Incorrect input, please enter correct value: ";
			}
		}
	}

	std::string dir{};
	{
		std::cout << "Enter directory to start search from: ";
		std::getline(std::cin, dir);
	}

	std::cout << "Searching for file \"" << filename << "\" in directory \"" << dir << "\" using " << number_of_threads << " threads\n";
	const auto start = std::chrono::system_clock::now();
	const auto results = FileSearcher::findFile(filename, number_of_threads, dir);
	const auto end = std::chrono::system_clock::now();
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	std::cout << "Search time: " << seconds << "s\n";
	std::cout << "Found " << results.size() << " files\n\n";

	for (const auto& result : results)
	{
		std::cout << result << "\n";
	}

	std::cin.get();

	return 0;
}

#endif

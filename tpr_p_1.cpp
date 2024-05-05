#include <iostream>
#include <thread>
#include <filesystem>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <atomic>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

int number_of_threads{ 4 };

std::mutex new_dir_m;
std::condition_variable new_dir_cv;
std::atomic_int number_of_dirs{ 0 };

std::mutex thread_finished_m;
std::condition_variable thread_finished_cv;
std::atomic_int number_of_finished_threads{ 0 };


std::queue<std::string> dirs{};
std::mutex dirs_m{};

std::string search_file{};

std::mutex results_m{};
std::vector<std::string> results{};

bool should_stop{ false };

void per_thread(int i);


void searchFile()
{
	dirs = {};
	results = {};
	number_of_finished_threads = 0;
	number_of_dirs = 0;
	should_stop = false;

	dirs.push("C:\\");

	std::vector<std::thread> threads{};
	for (int i = 0; i < number_of_threads; i++)
	{
		threads.emplace_back(std::thread(per_thread, i));
	}

	++number_of_dirs;
	new_dir_cv.notify_one();

	int free_threads{ number_of_threads - 1 };
	while (true)
	{
		{
			std::unique_lock lk(thread_finished_m);
			thread_finished_cv.wait(lk, [] { return number_of_finished_threads > 0; });
			--number_of_finished_threads;
			free_threads++;
		}

		{
			std::unique_lock lk(dirs_m);
			if (dirs.empty() && free_threads == number_of_threads)
			{
				number_of_dirs = number_of_threads;
				should_stop = true;
				new_dir_cv.notify_all();
				break;
			}
			else if(dirs.size())
			{
				free_threads--;
				new_dir_cv.notify_one();
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

void per_thread(int i)
{
	while (true)
	{
		{
			std::unique_lock lk(new_dir_m);
			new_dir_cv.wait(lk, [] { return number_of_dirs > 0; });
			number_of_dirs--;
		}

		if (should_stop)
		{
			return;
		}

		std::string dir{};
		{
			std::unique_lock lk(dirs_m);
			dir = dirs.front();
			dirs.pop();
		}

		// std::cout << "i: " << i << " " << dir << "\n";
		
		try
		{
			for (auto const& dir_entry : std::filesystem::directory_iterator{ dir })
			{
				if (dir_entry.is_regular_file())
				{
					std::string fname = dir_entry.path().filename().string();
					if (fname == search_file)
					{
						std::string path = dir_entry.path().string();
						{
							std::unique_lock lk(results_m);
							results.push_back(path);
						}
					}
				}
				else if (dir_entry.is_directory())
				{
					std::string next_dir = dir_entry.path().string();
					{
						std::unique_lock lk(dirs_m);
						dirs.push(next_dir);
						number_of_dirs++;
					}
				}
			}
		}
		catch (...)
		{

		}

		// std::cout << "i: " << i << "finished\n";
		number_of_finished_threads++;
		thread_finished_cv.notify_one();
	}
}

int main()
{
	number_of_threads = 4;
	std::cout << "number_of_threads: " << number_of_threads << "\n";
	{
		const auto start = std::chrono::system_clock::now();
		search_file = "main.cpp";
		std::cout << "Starting search for file \"" << search_file << "\"\n";
		searchFile();
		const auto end = std::chrono::system_clock::now();

		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
		std::cout << "Search time: " << seconds << "s\n";

		std::cout << "Number of \"" << search_file << "\" files found: " << results.size() << "\n";
		for (const auto& result : results)
		{
			//std::cout << result << '\n';
		}
	}

	std::cin.get();

	return 0;
}


#include "ClientService.h"
#include "SearchForFileRequest.h"
#include "SearchForFileResponse.h"

int ClientService::run()
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

	SearchForFileRequestData data{};
	data.file_name = filename;
	data.number_of_threads = number_of_threads;
	data.start_dir = dir;

	SearchForFileRequest req{};
	req.setData(data);
	req.serilize();

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(Config::getInstance().getDefaultServerIp(), Config::getInstance().getDefaultServerPort());
	if (status != sf::Socket::Done)
	{
		LOG("error", "failed to connect");
		return 0;
	}

	auto pack = req.getSfPacket();
	status = socket.send(pack);
	if (status != sf::Socket::Done)
	{
		LOG("error", "failed to send request");
		return 0;
	}
	LOG("info", "requst sent");

	status = socket.receive(pack);
	if (status != sf::Socket::Done)
	{
		LOG("error", "failed to send request");
		return 0;
	}

	SearchForFileResponse response{};
	response.setSfPacket(pack);
	if (!response.deserialize())
	{
		LOG("error", "failed to deserialize packet");
	}

	auto response_data = response.getData();
	std::cout << "Found " << response_data.results.size() << " files.";
	{
		size_t i{};
		for (const auto& url : response_data.results)
		{
			++i;
			try
			{
				std::cout << i << ". " << url.string() << "\n";
			}
			catch(...)
			{
				std::cout << i << ". " << "incorrect path" << "\n";
			}
		}
	}

	std::cin.get();

	return 0;
}

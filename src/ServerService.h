#pragma once
#include "FileSearcher.h"
#include "SearchForFileRequest.h"
#include <list>
#include <memory>
#include <Log.h>
#include "PacketBaseFactory.h"
#include "UserOnServer.h"
#include <atomic>

struct UserAndRequest
{
	uint64_t user_id{};
	std::unique_ptr<PacketBase> request{};
};

class ServerService
{
public:
	int run();

	~ServerService();
private:
	sf::TcpListener _listener;
	bool _running{ true };

	std::list<UserOnServer> _users;
	std::unordered_map<uint64_t, UserOnServer*> _id_to_user{};
	std::mutex _users_m{};

	std::mutex _request_queue_m{};
	std::queue<UserAndRequest> _request_queue{};
	Semaphore _request_sem{};

	std::thread _processor_thread{};

	void requestProcessorThread();

};

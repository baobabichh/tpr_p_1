#pragma once
#include "SFML/Network.hpp"
#include <memory>

struct UserOnServer
{
	uint64_t id{0};
	std::unique_ptr<sf::TcpSocket> socket{ nullptr };
	bool has_request{false};

	static const uint64_t getNextID();
};

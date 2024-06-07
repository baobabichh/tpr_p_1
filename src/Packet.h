#pragma once
#include <SFML/Network.hpp>

class PacketBase
{
public:
	void setSfPacket(const sf::Packet& packet);
	const sf::Packet& getSfPacket()const;
	virtual bool serilize();
	virtual bool deserialize();
	~PacketBase() = default;

protected:
	sf::Packet _packet{};
};

size_t getPacketTypeFromSfPacket(const sf::Packet& packet);

#define typeof_this std::remove_pointer<decltype(this)>::type
#define typeof_pointer(ptr) std::remove_pointer<decltype(ptr)>::type

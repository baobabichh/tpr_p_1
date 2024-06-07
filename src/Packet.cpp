#include "Packet.h"

void PacketBase::setSfPacket(const sf::Packet& packet)
{
	_packet = packet;
}

const sf::Packet& PacketBase::getSfPacket() const
{
	return _packet;
}

bool PacketBase::serilize()
{
	return false;
}

bool PacketBase::deserialize()
{
	return false;
}

size_t getPacketTypeFromSfPacket(const sf::Packet& packet)
{
	auto tmp = packet;
	size_t res{};
	if ((tmp >> res))
	{
		return res;
	}
	return 0;
}

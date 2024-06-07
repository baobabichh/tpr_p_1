#include "PacketBaseFactory.h"
#include "Log.h"

static std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>()>> _type_to_create_func;

std::unique_ptr<PacketBase> PacketBaseFactory::createPacketOfType(const size_t type)
{
	if (!getPacketBaseCreators().count(type))
	{
		LOG("error", "PacketBaseFactory uncnown packet type");
		return {};
	}
	return getPacketBaseCreators()[type]();
}

size_t& PacketBaseFactory::getPacketTypeGlobal()
{
	static size_t counter{ 10 };
	return counter;
}

std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>()>>& PacketBaseFactory::getPacketBaseCreators()
{
	static std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>()>> s;
	return s;
}

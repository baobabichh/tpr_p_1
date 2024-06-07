#pragma once

#include <unordered_map>
#include <functional>
#include "Packet.h"

class PacketBaseFactory
{
public:
	template <class T>
	static const size_t getPacketType()
	{
		static size_t counter = getPacketTypeGlobal()++;
		return counter;
	}

	template <class T>
	static const void initPacketType()
	{
		const auto type = getPacketType<T>();
		getPacketBaseCreators()[type] = []() -> std::unique_ptr<PacketBase> {return std::make_unique<T>(); };
	}

	static std::unique_ptr<PacketBase> createPacketOfType(const size_t type);

private:
	static size_t& getPacketTypeGlobal();
	static std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>()>>& getPacketBaseCreators();
};
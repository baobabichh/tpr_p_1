#pragma once
#include "Packet.h"

template <class Data>
class PacketBaseWithData : public PacketBase
{
public:
	void setData(const Data& data)
	{
		_data = data;
	}
	const Data& getData()const
	{
		return _data;
	}

protected:
	Data _data{};
};
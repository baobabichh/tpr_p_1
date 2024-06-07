#include "SearchForFileRequest.h"
#include "PacketBaseFactory.h"


bool SearchForFileRequest::serilize()
{
	_packet << PacketBaseFactory::getPacketType<typeof_this>() << _data.file_name.wstring() << _data.number_of_threads << _data.start_dir.wstring();

	return true;
}

bool SearchForFileRequest::deserialize()
{
	sf::Packet pack = _packet;
	SearchForFileRequestData tmp{};
	size_t pack_type{};

	std::wstring w_file_name{}, w_start_dir{};


	if (!(pack >> pack_type >> w_file_name >> tmp.number_of_threads >> w_start_dir))
	{
		return false;
	}

	if (PacketBaseFactory::getPacketType<typeof_this>() != pack_type)
	{
		return false;
	}

	tmp.file_name = w_file_name;
	tmp.start_dir = w_start_dir;


	_data = tmp;

	return true;
}

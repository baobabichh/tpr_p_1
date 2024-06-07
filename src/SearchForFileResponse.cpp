#include "SearchForFileResponse.h"

bool SearchForFileResponse::serilize()
{
	_packet << _data.results.size();
	for (const auto& it : _data.results)
	{
		_packet << it.wstring();
	}

	return true;
}

bool SearchForFileResponse::deserialize()
{
	size_t size{};
	if (!(_packet >> size))
	{
		return false;
	}
	_data.results.resize(size);
	for (auto& it : _data.results)
	{
		std::wstring str{};
		if (!(_packet >> str))
		{
			return false;
		}
		it = str;
	}

	return true;
}

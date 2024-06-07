#pragma once
#include "PacketBaseWithData.h"
#include <vector>
#include <filesystem>

struct SearchForFileResponseData
{
	std::vector<std::filesystem::path> results{};
};

class SearchForFileResponse : public PacketBaseWithData<SearchForFileResponseData>
{
public:
	virtual bool serilize() override;
	virtual bool deserialize() override;
};
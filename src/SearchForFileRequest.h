#pragma once
#include "PacketBaseWithData.h"
#include <filesystem>

struct SearchForFileRequestData
{
	std::filesystem::path file_name{};
	size_t number_of_threads{};
	std::filesystem::path start_dir{};
};

class SearchForFileRequest : public PacketBaseWithData<SearchForFileRequestData>
{
public:
	virtual bool serilize() override;
	virtual bool deserialize() override;
};

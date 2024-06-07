#include "RequestProcessor.h"
#include "PacketBaseFactory.h"

std::unique_ptr<PacketBase> RequestProcessor::processRequest(const std::unique_ptr<PacketBase>& request)
{
	const size_t request_type = getPacketTypeFromSfPacket(request->getSfPacket());

	if (getPacketProcessorFunctions().count(request_type))
	{
		return getPacketProcessorFunctions()[request_type](request);
	}

	return {};
}

void RequestProcessor::initAllProcessors()
{
	initProcessor<SearchForFileRequest>();
}

std::unique_ptr<PacketBase> RequestProcessor::processRequestImpl(const SearchForFileRequest& request)
{
	const auto& data = request.getData();
	auto results = FileSearcher::findFile(data.file_name, data.number_of_threads, data.start_dir);

	SearchForFileResponseData response_data{};
	response_data.results = results;

	SearchForFileResponse response{};
	response.setData(response_data);
	response.serilize();

	std::unique_ptr<PacketBase> res_ptr = std::make_unique<SearchForFileResponse>(response);
	return res_ptr;
}

std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>(const std::unique_ptr<PacketBase>& request)>>& RequestProcessor::getPacketProcessorFunctions()
{
	static std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>(const std::unique_ptr<PacketBase>& request)>> s{};
	return s;
}

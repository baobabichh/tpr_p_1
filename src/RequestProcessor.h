#pragma once

#include "FileSearcher.h"
#include "SearchForFileRequest.h"
#include "SearchForFileResponse.h"
#include "UserOnServer.h"
#include "PacketBaseFactory.h"

class RequestProcessor
{
public:
	static std::unique_ptr<PacketBase> processRequest(const std::unique_ptr<PacketBase>& request);

	static void initAllProcessors();

private:
	template <class T>
	static void initProcessor()
	{
		const size_t request_type = PacketBaseFactory::getPacketType<T>();
		getPacketProcessorFunctions()[request_type] = [](const std::unique_ptr<PacketBase>& request) { const T& request_ref = *(const T*)request.get(); return processRequestImpl(request_ref); };
	}

	static std::unique_ptr<PacketBase> processRequestImpl(const SearchForFileRequest& request);

	static std::unordered_map<size_t, std::function<std::unique_ptr<PacketBase>(const std::unique_ptr<PacketBase>& request)>>& getPacketProcessorFunctions();
};
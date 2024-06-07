#if defined(WIN64)
boost::filesystem::path::imbue(
	std::locale(std::locale(), new std::codecvt_utf8_utf16<wchar_t>()));
#endif

#include "ClientService.h"
#include "ServerService.h"
#include "SearchForFileRequest.h"
#include "SearchForFileResponse.h"
#include "RequestProcessor.h"

//#define RUN_TESTS

#ifdef RUN_TESTS
#include "Tests.h"
int main()
{
	return !runAllTests();
}
#else

int main()
{
	PacketBaseFactory::initPacketType<PacketBase>();
	PacketBaseFactory::initPacketType<SearchForFileRequest>();
	PacketBaseFactory::initPacketType<SearchForFileResponse>();
	RequestProcessor::initAllProcessors();

	std::cout << "Enter to run application S (Server) or C (Client): ";
	std::string choise{};
	while (std::getline(std::cin, choise))
	{
		if (choise == "C" || choise == "c")
		{
			ClientService client{};
			return client.run();
		}
		else if (choise == "S" || choise == "s")
		{
			ServerService server{};
			return server.run();
		}
		std::cout << "Enter S or C: ";
	}
}

#endif

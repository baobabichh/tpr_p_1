#include "UserOnServer.h"

const uint64_t UserOnServer::getNextID()
{
    static std::atomic<uint64_t> s{ 1 };
    return ++s;
}
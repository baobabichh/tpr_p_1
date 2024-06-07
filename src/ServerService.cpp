#include "ServerService.h"

#include "PacketBaseFactory.h"
#include "RequestProcessor.h"

int ServerService::run()
{
    _processor_thread = std::move(std::thread(&ServerService::requestProcessorThread, this));

    _listener.listen(Config::getInstance().getDefaultServerPort());
    sf::SocketSelector selector;
    selector.add(_listener);

    while (_running)
    {
        if (selector.wait())
        {
            if (selector.isReady(_listener))
            {
                std::unique_ptr<sf::TcpSocket> client = std::make_unique<sf::TcpSocket>();
                if (_listener.accept(*client) == sf::Socket::Done)
                {
                    std::lock_guard lk{ _users_m };

                    selector.add(*client);

                    UserOnServer user{};
                    user.id = UserOnServer::getNextID();
                    user.socket = std::move(client);
                    const auto id = user.id;

                    _users.push_back(std::move(user));
                    _id_to_user[id] = &_users.back();

                    LOG("info", "new connnection");
                }
                else
                {
                    LOG("info", "failed to get connection");
                }
            }
            else
            {
                for (auto it = _users.begin(); it != _users.end(); ++it)
                {
                    sf::TcpSocket& user_socket = *it->socket;
                    if (selector.isReady(user_socket))
                    {
                        // The client has sent some data, we can receive it
                        sf::Packet packet;
                        if (user_socket.receive(packet) == sf::Socket::Done && packet.getDataSize())
                        {
                            const auto packet_type = getPacketTypeFromSfPacket(packet);
                            if (!packet_type)
                            {
                                LOG("error", "uncnown packet type");
                                continue;
                            }
                            std::unique_ptr<PacketBase> packet_base_ptr = PacketBaseFactory::createPacketOfType(packet_type);

                            packet_base_ptr->setSfPacket(packet);
                            if (!packet_base_ptr)
                            {
                                LOG("error", "!packet_base_ptr");
                                continue;
                            }

                            if (!packet_base_ptr->deserialize())
                            {
                                LOG("error", "failed to deserilize packet");
                                continue;
                            }

                            {
                                std::lock_guard lk{ _request_queue_m };
                                UserAndRequest user_nad_req{};
                                user_nad_req.user_id = it->id;
                                user_nad_req.request = std::move(packet_base_ptr);
                                _request_queue.push(std::move(user_nad_req));
                                LOG("info", "enqueued new task");
                            }

                            _request_sem.post();
                        }
                        else
                        {
                            std::lock_guard lk{ _users_m };
                            selector.remove(user_socket);
                            _id_to_user.erase(it->id);
                            _users.erase(it);
                            LOG("info", "user disconnected");
                            break;
                        }
                    }
                }
            }
        }
    }

	return 0;
}

void ServerService::requestProcessorThread()
{
    while (true)
    {
        _request_sem.wait();

        if(!_running)
        {
            continue;
        }

        UserAndRequest task{};
        {
            std::lock_guard lk{ _request_queue_m };
            task = std::move(_request_queue.front());
            _request_queue.pop();
        }

        if (!task.request)
        {
            LOG("error", "empty task.request");
            continue;
        }

        LOG("info", "starting process task");

        auto response = RequestProcessor::processRequest(task.request);

        LOG("info", "finished process task");

        {
            std::lock_guard lk{ _users_m };
            if (!_id_to_user.count(task.user_id))
            {
                continue;
            }
            response->serilize();
            auto pack = response->getSfPacket();

            auto* user = _id_to_user[task.user_id];
            if (!user)
            {
                continue;
            }
            user->socket->send(pack);
            LOG("info", "send result to user");
        }
    }
}

ServerService::~ServerService()
{
    if (_processor_thread.joinable())
    {
        _processor_thread.join();
    }

    for (auto& user : _users)
    {
        user.socket->disconnect();
    }
}

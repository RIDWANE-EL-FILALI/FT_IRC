#include "../../inc/Server.hpp"

DDD::DDD(Server *server) : Command(server)
{
}

DDD::~DDD()
{
}

void DDD::run(Client* client, std::list<std::string> args) {
    std::string client_name;
    std::string file_path;

    if (args.empty() || args.size() < 2)
    {
        client->reply(Replies::ERR_NEEDMOREPARAMS("DDD"));
        return ;
    }
    client_name = args.front();
    args.pop_front();
    file_path = args.front();
    args.pop_front();

    std::cout << "client: " << client_name << std::endl;
    std::cout << "file_path: " << file_path << std::endl;

    server->transfer_file(client_name, file_path);
}
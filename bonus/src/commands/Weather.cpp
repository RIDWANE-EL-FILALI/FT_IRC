#include "../../inc/Server.hpp"

Weather::Weather(Server *server): Command(server) {
}

Weather::~Weather() {
}


void Weather::run(Client* client, std::list<std::string> args) {
    std::string city = args.front();
    
    std::cout << "trying to fetch weather for " << city << std::endl;
    Client *c = server->getClientByNickname("bot");
    if (c) {
        c->reply("PING");
    }
    else
        client->reply("No bot found.");
    return ;
}
    
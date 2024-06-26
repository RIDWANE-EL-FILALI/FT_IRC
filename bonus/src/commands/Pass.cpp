#include "../../inc/Server.hpp"

Pass::Pass(Server *server): Command(server) {
    
}

Pass::~Pass() {

}

void Pass::run(Client* client, std::list<std::string> args) {
    if (client->getState() == REGISTERED) {
        client->reply(Replies::ERR_ALREADYREGISTERED());
        return;
    }

    if (args.size() < 1)
        client->reply(Replies::ERR_NEEDMOREPARAMS("PASS"));
    std::string password = args.front().substr(0, args.front().length());
    if (this->server->getPass() != password) {
        client->reply(Replies::ERR_PASSWDMISMATCH());
        return;
    }

    client->setState(REGISTERED);
}

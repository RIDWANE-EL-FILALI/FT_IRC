#include "../../inc/Server.hpp"

Nick::Nick(Server *server): Command(server) {
    
}

Nick::~Nick() {
    
}

void Nick::run(Client* client, std::list<std::string> args) {
    if (client->getState() == REGISTERED) {
        std::cout << "Already registered\n";
        client->reply(Replies::ERR_ALREADYREGISTERED());
        return;
    }

    if (args.size() < 1)
        client->reply(Replies::ERR_NEEDMOREPARAMS("NICK"));

    std::string nickname = args.front();
    if (nickname.length() < 1 || nickname.length() > 9) {
        client->reply(Replies::ERR_NOSUCHNICK(nickname));
        return;
    }

    if (this->server->checkNickname(nickname)) {
        client->reply(Replies::ERR_NICKNAMEINUSE(nickname));
        return;
    }

    client->setNickname(nickname);
}
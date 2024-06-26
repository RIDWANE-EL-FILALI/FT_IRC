#include "inc/Server.hpp"

int main(int ac, char **av)
{
    try{
        if (ac != 3)
            throw std::runtime_error("Usage: /ircserv <port> <password>");
        Server _server(av[1], av[2]);
        _server.start(av[2]);
    }
    catch(std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
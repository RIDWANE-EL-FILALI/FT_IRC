#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread> //c++11 not c++98!!
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <map>
#include <list>
#include <netdb.h>
#include <sstream>
#include "Client.hpp"
#include "CommandHandler.hpp"
#include "channel.hpp"
#include "Replies.hpp"
#include "Command.hpp"

//added headers for clean code
#include <algorithm>
#include "time.h"

class CommandHandler;

class Server{
    private:
        int sock_fd; //fd dyal server
        std::string port;
        std::string pass;
        std::vector<pollfd> _pollfds;
        std::map<int, Client *> _clients;
        CommandHandler *_commandHandler;
        std::map<std::string, channel *> channels;

    public:
        Server(std::string port, std::string pass);
        ~Server();
        void createSocket(std::string port);

        void start(std::string pass);
        void addClient(int fd);
        void removeClient(int fd);
        void handleMessage(int fd);
        std::string readMessage(int fd);
        // void sendMessage(int fd, std::string message);

        std::string getPass() const;
        bool checkNickname(std::string const nickname) const;


        // part related to the channels
        void joinChannel(std::string& channelName, Client* client, std::string& key);
        static void broadcast(std::string &targer, Client* client, std::string &message, Server *server);
        static void direct_message(std::string &targer, Client* client, std::string &message, Server *server);
        void broadcast_joining(std::string channelName, Client *client, Server *server);
        static void kickUser(std::string channelName, Client *client, std::string nickname, Server *server, std::string comment);
        static void kickUser(std::string channelName, Client *client, std::list<std::string> &userList, Server *server, std::string comment);
        void broadcast_kick(std::string channelName, Client *client, std::string nickname, Server *server, std::string comment);
        void partUser(std::string channelName, Client *client, Server *server, std::string comment);
        void quitUser(Client *client, std::string message, Server *server);

		bool setInviteOnlyChannel(std::string channelName, Client* client, bool adding);
		bool setTopicRestrictedChannel(std::string channelName, bool adding, Client *client);
		bool setPasswordChannel(std::string channelName, bool adding, std::string key, Client *client);
		bool setUserLimitChannel(std::string channelName, bool adding, std::string limit, Client *client);
		bool setOperatorChannel(std::string channelName, std::string targetClient, bool adding, Client *client);
		void replyChannelModIs(std::string channelName, Client *client, std::map<char, std::string>	args);
		void broadcastArgsReply(std::string channelName, std::string modeReply, Client *client);
		std::map<std::string, channel *>	getChannels();
		channel*							getChannel(std::string channelName);
		Client*								getMember(std::string nickname);
};

#endif
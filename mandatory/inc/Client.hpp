#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class channel;

enum ClientState{
    UNREGISTERED,
    REGISTERED,
    AUTHENTICATED,
    AWAY,
    OPERATOR,
    UNKNOWN
};

class Client{
    private:
        int fd;
        int port;
        ClientState state;
        std::string hostname;
        std::string username;
        std::string nickname;
        std::string realname;
		std::vector<channel*>	invitingChannels; // the channels that invited the client (used in invite command)

    public:
        Client(std::string hostname, int port, int fd);
        ~Client();

        std::string getHostname() const;
        std::string getUsername() const;
        std::string getNickname() const;
        std::string getRealname() const;
        int getState() const;
        int getPort() const;
        int getFd() const;

        void setUsername(std::string username);
        void setNickname(std::string nickname);
        void setRealname(std::string realname);
        void setState(ClientState state);

        void reply(std::string message);

        void welcomeToServer();

		std::vector<channel*>	getInvitingChannels();
		channel*				getInvitingChannel(std::string channelName);
		void					addInvitingChannel(channel *Channel);
};

#endif
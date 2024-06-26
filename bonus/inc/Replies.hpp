#ifndef REPLIES_HPP
#define REPLIES_HPP

#include "Server.hpp"

class Replies {
public:

    // REPLIES
    static std::string RPL_WELCOME(const std::string& nickname, const std::string& username, const std::string& hostname);
    // ERRORS
    static std::string ERR_UNKNOWNCOMMAND(const std::string& command);
    static std::string ERR_NEEDMOREPARAMS(const std::string& command);
    static std::string ERR_NOSUCHNICK(const std::string& nickname);
    static std::string ERR_NICKNAMEINUSE(const std::string& nickname);
    static std::string ERR_PASSWDMISMATCH();
    static std::string ERR_ALREADYREGISTERED();
    static std::string ERR_ONLYONECHANNEL(const std::string& user);
    static std::string ERR_BADCHANMASK(const std::string& channel);
    static std::string ERR_ALREADYINCHANNEL(const std::string& nickname);
    static std::string ERR_ALREADYOPERATOR(const std::string& nickname);
    static std::string ERR_BADCHANNELKEY(const std::string& channel, Client *client);
    static std::string ERR_NORECIPIENT(const std::string& nickname);
    static std::string ERR_NOTONCHANNEL(const std::string& nickname, const std::string& channel);
    static std::string ERR_USERNOTINCHANNEL(const std::string& operator_nick, const std::string& channel, const std::string& nonexistent_user);
    static std::string ERR_INVITEONLYCHAN(const std::string& nickname);
    static std::string ERR_CHANNELISFULL(const std::string& nickname);
    static std::string ERR_CHANOPRIVSNEEDED(const std::string& channel, const std::string& nickname);
    static std::string ERR_CANNOTKICKOPERATOR(const std::string& channelName);

	static std::string RPL_CHANNELMODEIS (std::string& channelName, std::string& command);
	static std::string ERR_USERONCHANNEL (Client *client, std::string channelName);
	static std::string ERR_NOSUCHCHANNEL (std::string& channelName);
	static std::string ERR_UNKNOWNMODE   (char& modeChar);
	static std::string RPL_INVITING      (std::string channelName, Client *client, Client* invitedMember, int sendToClient);
	static std::string ERR_NOTREGISTERED ();
	static std::string ERR_ERRONEUSNICKNAME (std::string& nickname);
	static std::string ERR_USERNOTINCHANNEL  (std::string targetClient, std::string channelName);
	static std::string ERR_CANTKICKCREATOR (Client *client, std::string channelName);
	static std::string ERR_INVALIDLIMIT (Client *client, std::string channelName);
	static std::string ERR_CANTINVITESELF(std::string clientName);
	static std::string RPL_INVITELIST(const std::string& clientNickname, std::vector<channel*> channels);
	static std::string RPL_TOPIC(std::string clientName, std::string channelName, std::string topic);
	static std::string RPL_TOPICWHOTIME(std::string clientName, std::string channelName, std::string topicSetter, time_t setTime);
};

#endif

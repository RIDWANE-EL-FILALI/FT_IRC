#include "../inc/Server.hpp"

Server::Server(std::string port, std::string pass)
{
    this->port = port;
    this->pass = pass;
    createSocket(port);
    _commandHandler = new CommandHandler(this);
}

Server::~Server()
{
    delete _commandHandler;
}

void Server::createSocket(std::string port)
{
    this->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sock_fd == -1)
    {
        close(this->sock_fd);
        throw std::runtime_error("can't create socket");
    }

    sockaddr_in ServerAddress = {};
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_addr.s_addr = INADDR_ANY;
    ServerAddress.sin_port = htons(std::atoi(port.c_str()));

    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK) == -1)
    {
        close(sock_fd);
        throw std::runtime_error("can't set non-blocking");
    }

    if (bind(sock_fd, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress)) == -1){
        close(this->sock_fd);
        throw std::runtime_error("can't bind socket");
    }

    if (listen(sock_fd, 10) == -1)
    {
        close(sock_fd);
        throw std::runtime_error("error listening");
    }
}

void Server::addClient(int sock_fd)
{
    int client_fd;
    sockaddr_in clientAddress = {};
    socklen_t clientAddressSize = sizeof(clientAddress);
    
    client_fd = accept(sock_fd, (struct sockaddr*)&clientAddress, &clientAddressSize);
    if (client_fd == -1)
        throw std::runtime_error("can't accept client");
    
    pollfd client_poll = {client_fd, POLLIN, 0};
    _pollfds.push_back(client_poll);

    char hostname[NI_MAXHOST];
    int result = getnameinfo((struct sockaddr*)&clientAddress, sizeof(clientAddress), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);

    if (result != 0) {
        close(client_fd);
        throw std::runtime_error("can't get hostname");
    }

    Client *client = new Client(hostname, ntohs(clientAddress.sin_port), client_fd);
    _clients.insert(std::make_pair(client_fd, client));

    //TODO: log client connection
}

std::string Server::readMessage(int fd)
{
    char buffer[1024];
    std::string message;
    int bytes = recv(fd, buffer, 1024, 0);
    if (bytes == -1)
        throw std::runtime_error("can't read message");
    else
        message = std::string(buffer, bytes);
    return message;
}

void Server::handleMessage(int fd)
{
    try
    {
        Client* client = _clients.at(fd);
        std::string message = readMessage(fd);
        _commandHandler->handleCommand(message, client);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        throw std::runtime_error("error handling message");
    }
}

void Server::start(std::string pass){

    pollfd serverfd = {sock_fd, POLLIN, 0};
    _pollfds.push_back(serverfd);

    std::cout << "server is on " << this->port << std::endl;

    while(true)
    {
        if (poll(_pollfds.data(), _pollfds.size(), -1) == -1) {
            throw std::runtime_error("poll error");
        }

        for(std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
        {
            if (it->revents == 0)
                continue;

            if (it->revents & POLLIN)
            {
                if(it->fd == sock_fd) {
                    addClient(sock_fd);
                    break;
                }
                handleMessage(it->fd);
            }
        }
    }
}

std::string Server::getPass() const
{
    return this->pass;
}

bool Server::checkNickname(std::string const nickname) const
{
    for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNickname() == nickname)
            return true;
    }
    return false;
}

void Server::joinChannel(std::string& channelName, Client* client, std::string& key) {
    std::map<std::string, channel *>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        channels.insert(std::make_pair(channelName, new channel(client, channelName, key)));
        std::map<std::string, channel *>::iterator it = channels.find(channelName);
        client->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channelName + " " + key);
        std::string mode;
        if (it->second->getInviteOnly()) {
            mode += "i";
        }
        if (it->second->getTopicRestricted()) {
            mode += "t";
        }
        if (it->second->getUsersLimit() != -1) {
            mode += "l";
        }
        if (it->second->getKey() != "") {
            mode += "k";
        }
        if (mode != "")
            client->reply(":localhost MODE " + channelName + " +" + mode);
        else
            client->reply(":localhost MODE " + channelName);
        std::map<std::string, Client *> test = channels[channelName]->getMembers();
        std::string members;
        for(std::map<std::string, Client *>::iterator it = test.begin(); it != test.end(); it++)
        {
            if (channels[channelName]->isOperator(it->first))
                members += "@" + it->first + " ";
            else
                members += it->first + " ";
        }
        client->reply(":localhost 353 " + client->getNickname() + " = " + channelName + " :" + members);
        client->reply(":localhost 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list.");
        return ;
    }
    channel* channel = it->second;
    if (channel->getUsersLimit() != -1 && channel->getMembers().size() >= channel->getUsersLimit()) {
        client->reply(Replies::ERR_CHANNELISFULL(channelName));
        return;
    }
    if (channel->getInviteOnly() && !channel->isMember(client->getNickname())) {
        client->reply(Replies::ERR_INVITEONLYCHAN(channelName));
        return;
    }
    if (!channel->getKey().empty() && channel->getKey() != key) {
        client->reply(Replies::ERR_BADCHANNELKEY(channelName, client));
        return;
    }
    channel->addMember(client->getNickname(), client);

    // handle case where the channel already exist reply 
    client->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN " + channelName + " " + key);
    client->reply(":localhost 332 " + client->getNickname() + " " + channelName + " :" + channel->getTopic());
    client->reply(":localhost 333 " + client->getNickname() + " " + channelName + " " + channel->getCreator() + " " + std::to_string(channel->getCreationTime()));
    std::map<std::string, Client *> test = channels[channelName]->getMembers();
    std::string members;
    for(std::map<std::string, Client *>::iterator it = test.begin(); it != test.end(); it++)
    {
        if (channels[channelName]->isOperator(it->first))
            members += "@" + it->first + " ";
        else
            members += it->first + " ";
    }
    client->reply(":localhost 353 " + client->getNickname() + " = " + channelName + " :" + members);
    client->reply(":localhost 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list.");
    broadcast_joining(channelName, client, this);
}

void Server::broadcast_joining(std::string channelName, Client *client, Server *server)
{
    std::map<std::string, channel *>::iterator it = server->channels.find(channelName);
    if (it == server->channels.end())
    {
        client->reply(Replies::ERR_NOSUCHNICK("PRIVMSG"));
        return ;
    }
    channel *channel = it->second;
    std::string message = ":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " JOIN :" + channelName;
    for (std::map<std::string, Client *>::iterator ti = channel->getMembers().begin(); ti != channel->getMembers().end();ti++)
    {
        if (ti->first == client->getNickname())
            continue;
        else if (channel->isMember(client->getNickname()) == true)
            ti->second->reply(message);
    }
}


// change breadcast message to be a function that takes the message and the server

void Server::broadcast(std::string &targer, Client* client, std::string &message, Server *server) {
    std::map<std::string, channel *>::iterator it = server->channels.find(targer);
    if (it == server->channels.end())
    {
        client->reply(Replies::ERR_NOSUCHNICK("PRIVMSG"));
        return ;
    }
    channel *channel = it->second;
    if (channel->isMember(client->getNickname()) == false)
    {
        client->reply(Replies::ERR_NOTONCHANNEL(client->getNickname(), targer));
        return ;
    }
    for (std::map<std::string, Client *>::iterator ti = channel->getMembers().begin(); ti != channel->getMembers().end();ti++)
    {
        if (ti->first == client->getNickname())
            continue;
        else
            ti->second->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + targer + " :" + message);
    }
}

void Server::direct_message(std::string &targer, Client* client, std::string &message, Server *server) {
    std::map<int, Client *> clients = server->_clients;
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); it++)
    {
        if (it->second->getNickname() == targer)
        {
            it->second->reply(":" + client->getNickname() + " PRIVMSG " + targer + " :" + message);
            return ;
        }
    }
    client->reply(Replies::ERR_NOSUCHNICK("PRIVMSG"));
}


void Server::kickUser(std::string channelName, Client *client, std::string nickname, Server *server, std::string comment)
{
    std::map<std::string, channel *>::iterator it = server->channels.find(channelName);
    if (it == server->channels.end())
    {
        client->reply(Replies::ERR_NOSUCHNICK("KICK"));
        return ;
    }
    channel *channel = it->second;
    if (channel->isMember(nickname) == false)
    {
        client->reply(Replies::ERR_NOTONCHANNEL(nickname, channelName));
        return ;
    }
    if (channel->isOperator(client->getNickname()) == false)
    {
        client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
        return ;
    }
    if (channel->isOperator(nickname) == true)
    {
        client->reply(Replies::ERR_CANNOTKICKOPERATOR(channelName));
        return ;
    }
    channel->getMembers()[nickname]->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " KICK " + channelName + " " + nickname + " :" + comment);
    server->broadcast_kick(channelName, client, nickname, server, comment);
    channel->getMembers().erase(nickname);
}

void Server::kickUser(std::string channelName, Client *client, std::list<std::string> &userList, Server *server, std::string comment)
{
    std::string message;
    std::map<std::string, channel *>::iterator it = server->channels.find(channelName);
    if (it == server->channels.end())
    {
        client->reply(Replies::ERR_NOSUCHNICK("KICK"));
        return ;
    }
    channel *channel = it->second;
    if (channel->isOperator(client->getNickname()) == false)
    {
        client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
        return ;
    }
    for (std::list<std::string>::iterator it = userList.begin(); it != userList.end(); it++)
    {
        if (channel->isMember(*it) == false)
        {
            std::cout << "this has been trigered" << std::endl;
            client->reply(Replies::ERR_USERNOTINCHANNEL(client->getNickname(), channelName, *it));
            continue;
        }
        if (channel->isOperator(*it) == true)
        {
            client->reply(Replies::ERR_CANNOTKICKOPERATOR(channelName));
            return ;
        }
        channel->getMembers()[*it]->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " KICK " + channelName + " " + *it + " :" + comment);
        server->broadcast_kick(channelName, client, *it, server, comment);
        channel->getMembers().erase(*it);
    }
}

void Server::broadcast_kick(std::string channelName, Client *client, std::string nickname, Server *server, std::string comment)
{
    std::map<std::string, channel *>::iterator it = server->channels.find(channelName);
    for (std::map<std::string, Client *>::iterator ti = it->second->getMembers().begin(); ti != it->second->getMembers().end();ti++)
    {
        if (ti->first == nickname)
            continue;
        else
            ti->second->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " KICK " + channelName + " " + nickname + " :" + comment);
    }
}

void Server::partUser(std::string channelName, Client *client, Server *server, std::string comment) {
    std::map<std::string, channel *>::iterator it = server->channels.find(channelName);
    if (it == server->channels.end()) {
        client->reply(Replies::ERR_NOSUCHNICK("PART"));
        return ;
    }
    channel *channel = it->second;
    if (channel->isMember(client->getNickname()) == false) {
        client->reply(Replies::ERR_NOTONCHANNEL(client->getNickname(), channelName));
        return ;
    }
    std::string sender = client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
    if (channel->getCreator() == sender)
    {
        for(std::map<std::string, Client *>::iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
        {
            it->second->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName + " :Channel deleted by operator and left this comment --> " + comment);
        }
        server->channels.erase(channelName);
        return ;
    }
    if (channel->getMembers().size() == 1)
    {
        server->channels.erase(channelName);
        client->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName + " :" + comment);
        return ;
    }
    if (channel->isOperator(client->getNickname()) == true)
    {
        channel->getOperators().erase(std::remove(channel->getOperators().begin(), channel->getOperators().end(), client->getNickname()),channel->getOperators().end());
        channel->getMembers().erase(client->getNickname());
    }
    else
    {
        channel->getMembers().erase(client->getNickname());
    }
    client->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName + " :" + comment);
    for(std::map<std::string, Client *>::iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
    {
        it->second->reply(":" + client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname() + " PART " + channelName + " :" + comment);
    }
}

void Server::quitUser(Client *client, std::string message, Server *server) {
    for (std::map<std::string, channel *>::iterator it = server->channels.begin(); it != server->channels.end(); it++)
    {
        if (it->second->isMember(client->getNickname()) == true)
        {
            partUser(it->first, client, server, message);
            std::string sender = client->getNickname() + "!" + client->getUsername() + "@" + client->getHostname();
            if (it->second->getCreator() == sender || server->channels.size() == 0)
                break;
        }
    }
    client->reply(":ircserv.localhost 301 " + client->getNickname() + " :You have quit IRC");
    close(client->getFd());
    server->_clients.erase(client->getFd());
    std::cout << "client " << client->getNickname() << " has quit" << std::endl;
    return ;
}


bool Server::setInviteOnlyChannel(std::string channelName, Client* client, bool adding) 
{
	std::map<std::string, channel *>::iterator	it;
	std::string									found_op;

	it = channels.find(channelName);
	if (it->second->getChannelName() != channelName)
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	if (it->second->isOperator(client->getNickname()))
	{
    	it->second->setInviteOnly(adding);
	}
	else
	{
		client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
		return (false);
	}
	return (true);
}

bool Server::setTopicRestrictedChannel(std::string channelName, bool adding, Client *client)
{
	std::map<std::string, channel *>::iterator it;

	it = channels.find(channelName);
	if (it->second->getChannelName() != channelName)
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	if (it->second->isOperator(client->getNickname()))
	{
    	it->second->setTopicRestricted(adding);
	}
	else
	{
		client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
		return (false);
	}
	return (true);
}

bool Server::setPasswordChannel(std::string channelName, bool adding, std::string key, Client *client)
{
	std::map<std::string, channel *>::iterator it;

	it = channels.find(channelName);
	if (it->second->getChannelName() != channelName)
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	if (it->second->isOperator(client->getNickname()))
	{
    	it->second->setKey(key);
	}
	else
	{
		client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
		return (false);
	}
	return (true);
}

bool Server::setUserLimitChannel(std::string channelName, bool adding, std::string limit, Client *client)
{
	std::map<std::string, channel*>::iterator	it;
	int											_limit;

	it = channels.find(channelName);
	if (it->second->getChannelName() != channelName)
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	_limit = adding ? atoi(limit.c_str()) : -1;
	if (_limit < it->second->getMembers().size() && adding)
	{
		client->reply(Replies::ERR_INVALIDLIMIT(client, channelName));
		return (false);
	}
	it->second->setUsersLimit(_limit);
	return (true);
}


bool Server::setOperatorChannel(std::string channelName, std::string targetClient, bool adding, Client *client)
{
	std::map<std::string, channel*>::iterator _channel;
	Client* _client;

	_channel = this->getChannels().find(channelName);
	if (_channel == this->getChannels().end())
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(channelName));
		return (false);
	}
	_client = _channel->second->getClient(targetClient);
	if(_client == NULL)
	{
		client->reply(Replies::ERR_USERNOTINCHANNEL(targetClient, channelName));
		return (false);
	}
	if (!_channel->second->isOperator(client->getNickname()))
	{
		client->reply(Replies::ERR_CHANOPRIVSNEEDED(channelName, client->getNickname()));
		return (false);
	}
	if(_channel->second->isCreator(_client) && !adding)
	{
		client->reply(Replies::ERR_CANTKICKCREATOR(client, channelName));
		return (false);
	}
	{
		if (adding)
			return  (_channel->second->addOperator(_client->getNickname(), client));
		else
			return (_channel->second->removeOperator(_client->getNickname(), client));

	}
	return (true);
}

void Server::replyChannelModIs(std::string channelName, Client *client, std::map<char, std::string>	args)
{
    std::string modeReply = ""; // Initialize modeReply as an empty string
    std::map<std::string, channel *>::iterator it;
	bool multiple_ops = false;

    it = channels.find(channelName);
    if(it->second->getInviteOnly())
        modeReply += "i";
    if(it->second->getTopicRestricted())
        modeReply += "t";
    if(it->second->getKey() != "")
        modeReply += "k";
    if (it->second->getUsersLimit() > 0)
        modeReply += "l";
	if (it->second->getOperators().size() == 2)
		modeReply +="o";
	else if (it->second->getOperators().size() > 2)
	{
		modeReply +="oo";
		multiple_ops = true;
	}
    // Check if modeReply is still empty, if so, don't prepend "MODE +" to the reply
    if (!modeReply.empty()) {
        modeReply = "MODE +" + modeReply;
		for (int i = 0; i < modeReply.size() ; i++)
		{
			if (modeReply[i] == 'k' || modeReply[i] == 'l')
			{
				if (modeReply[i] == 'k')
					modeReply += " " + it->second->getKey();
				if (modeReply[i] == 'l')
					modeReply += " " + std::to_string(it->second->getUsersLimit());
			}
			if ((multiple_ops && modeReply[i] == 'o' && modeReply[i + 1] == 'o') 
			|| (!multiple_ops && modeReply[i] == 'o'))
			{
				for (std::vector<std::string>::iterator ops = it->second->getOperators().begin(); 
				ops != it->second->getOperators().end(); ops++)
				{
					std::cout << "OPERATORS ARE :: " + (*ops) << std::endl;
					if (!it->second->isCreator(it->second->getClient(*ops)))
						modeReply += " " + *ops;
				}
			}
		}
    }

    client->reply(Replies::RPL_CHANNELMODEIS(channelName, modeReply));
}

void Server::broadcastArgsReply(std::string channelName, std::string modeReply, Client *client)
{
	std::map<std::string, channel*>::iterator it;
	std::map<std::string, Client*>::iterator members;

	it = channels.find(channelName);
		
	for (members = it->second->getMembers().begin(); members != it->second->getMembers().end();
		members++)
	{	
		members->second->reply(Replies::RPL_CHANNELMODEIS(channelName, modeReply));
	}
}


std::map<std::string, channel *> Server::getChannels()
{
	return (this->channels);
}

Client*	Server::getMember(std::string nickname)
{
	std::map<int, Client*>::iterator clients;

	for (clients = this->_clients.begin(); clients != this->_clients.end(); ++clients)
	{
		if (clients->second->getNickname() == nickname)
			return (clients->second);
	}
	return (NULL);
}

channel* Server::getChannel(std::string channelName)
{
	std::map<std::string, channel*>::iterator Channel;

	for (Channel = this->channels.begin(); Channel != channels.end(); Channel++)
	{
		if (Channel->first == channelName)
			return (Channel->second);
	}
	return (NULL);
}

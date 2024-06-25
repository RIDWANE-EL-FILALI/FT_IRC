#include "../../inc/Server.hpp"

Topic::Topic(Server *server) : Command(server)
{

}

Topic::~Topic()
{

}

void Topic::run(Client* client, std::list<std::string> args)
{
	channel*	Channel;
	std::string	Topic;
	time_t		setTime = 0;

	if (args.empty() || *args.begin() == "TOPIC")
	{
		client->reply(Replies::ERR_NEEDMOREPARAMS("TOPIC"));
		return ;
	}
	Channel = server->getChannel(args.front());
	if (Channel == NULL)
	{
		client->reply(Replies::ERR_NOSUCHCHANNEL(args.front()));
		return ;
	}
	args.pop_front();
	if (Channel->getMembers().find(client->getNickname()) == Channel->getMembers().end())
	{
		client->reply(Replies::ERR_NOTONCHANNEL(client->getNickname(), 
		Channel->getChannelName()));
		return ;
	}
	if (Channel->getTopicRestricted() && !Channel->isOperator(client->getNickname()))
	{
		client->reply(Replies::ERR_CHANOPRIVSNEEDED(Channel->getChannelName(),
		client->getNickname()));
		return;
	}
	if (args.empty())
	{
		client->reply(Replies::RPL_TOPIC(client->getNickname(), Channel->getChannelName(), Channel->getTopic()));
		if (Channel->getTopic() != "")
			client->reply(Replies::RPL_TOPICWHOTIME(client->getNickname(), Channel->getChannelName()
		, Channel->getSetTopicSetter(""), setTime));
		return ;
	}
	if (args.front()[0] == ':')
	{
		args.front().erase(0, 1);
		Topic += args.front();
		args.pop_front();
		while (!args.empty())
		{
			Topic += " " + args.front();
			args.pop_front();
		}
	}
	else
	{
		Topic = args.front();
	}
	{
		Channel->setTopic(Topic);
		setTime = std::time(nullptr);
		Channel->getSetTopicSetter(client->getNickname());
		Channel->broadcastReply(Replies::RPL_TOPIC(client->getNickname(),
		Channel->getChannelName(), Channel->getTopic()));
	}
}
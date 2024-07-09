#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"

class channel {
    private:
        std::string name;
        std::string creator; // this variable saves the creator of the channel information in like this format "nickname!username@hostname"
        time_t creationTime; 
		std::string topicSetter;

		/*this was changed. because time_t is not found in the std namespace, 
		because time_t is a C-style type defined in <ctime> (or <time.h> for the C header). 
		In C++98, time_t is not in the std namespace.*/ 
		
        std::string topic;
        std::string key;
        bool inviteOnly; // implements is in the logic of the server ✅
        bool topicRestricted; // implements is in the logic of the server ✅
        int userLimit; // implements is in the logic of the server ✅
        std::map<std::string, Client *>	members;
		std::vector<std::string>		invitedMembers;
        std::vector<std::string>		operators;
    public:
        channel();
        channel(Client *client, std::string name, std::string key, std::string topic = "");
        ~channel();

        void addMember(std::string name, Client *client);

        bool isOperator(std::string name);
        bool isMember(std::string name);

        //setters and getters
        std::map<std::string, Client *>& getMembers(); // ✅
        std::vector<std::string>& getOperators(); // ✅
        std::string &getKey(); // ✅
        void setKey(std::string key); // ✅
        std::string &getTopic(); // ✅
        void setTopic(std::string topic); 
        time_t getCreationTime();
        void setUsersLimit(int limit);
        int getUsersLimit();
        bool getInviteOnly();
        void setInviteOnly(bool inviteOnly);
        bool getTopicRestricted();
        void setTopicRestricted(bool topicRestricted);
        std::string	getCreator();
		std::string	getChannelName();
        bool getInvitedMember(std::string name);

		std::vector<std::string>	getInvitedMembers();
		std::string					getSetTopicSetter(std::string topicSetter);
		Client*						getClient(std::string name);
        bool						addOperator(std::string name, Client *client);
		bool						removeOperator(std::string name, Client *client);
		void						addInvite(Client* client, Client *invitedClient);
		bool						isCreator(Client* client);
		void						broadcastReply(std::string Reply);
};





#endif
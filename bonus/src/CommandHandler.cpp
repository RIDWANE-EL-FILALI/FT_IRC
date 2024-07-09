#include "../inc/Server.hpp"

CommandHandler::CommandHandler(Server *server): server(server) {
    _commands["NICK"] = new Nick(server);
    _commands["USER"] = new User(server);
    _commands["PASS"] = new Pass(server);
    _commands["JOIN"] = new Join(server);
    _commands["PRIVMSG"] = new PrivMsg(server);
    _commands["KICK"] = new Kick(server);
    _commands["PART"] = new Part(server);
    _commands["QUIT"] = new Quit(server);
	_commands["MODE"] = new Mode(server);
	_commands["INVITE"] = new Invite(server);
	_commands["TOPIC"] = new Topic(server);
    _commands["WEATHER"] = new Weather(server);
}

CommandHandler::~CommandHandler(){

}

void CommandHandler::handleCommand(std::string command, Client *client) {
	std::cout << "Command : " << command << std::endl;
    if (command[0] == ':')
        command = command.substr(1);
    std::stringstream ss(command);
    std::string cmd;

    while(std::getline(ss, cmd)) {
        if (cmd.length() - 1 == '\r') {
            cmd = cmd.substr(0, cmd.length());
        } else {
            cmd = cmd.substr(0, cmd.length());
        }
        std::string commandName = cmd.substr(0, cmd.find(" "));
        try{
            Command *c = _commands.at(commandName);
            std::string argsBuffer = cmd.substr(cmd.find(" ") + 1);
            std::istringstream argsStream(argsBuffer);
            std::string arg;
            std::list<std::string> args;
            while(std::getline(argsStream, arg, ' ')) {
                arg.erase(std::remove_if(arg.begin(), arg.end(), ::isspace), arg.end());
                args.push_back(arg);
            }
			c->run(client, args);
        } catch (const std::out_of_range &e) {
            client->reply(Replies::ERR_UNKNOWNCOMMAND(commandName));
        }
    }
}

Command *CommandHandler::getCommand(std::string command){
    return _commands[command];
}
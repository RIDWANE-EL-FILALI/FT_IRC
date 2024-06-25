CPPFLAGS = -std=c++11
CC = g++ 
NAME = ircserv
PREFIX = ./obj/

SOURCE = 	src/commands/Nick.cpp \
			src/commands/Pass.cpp \
			src/commands/User.cpp \
			src/commands/Join.cpp \
			src/commands/Kick.cpp \
			src/commands/PrivMsg.cpp \
			src/commands/Part.cpp \
			src/commands/Quit.cpp \
			src/commands/Mode.cpp \
			src/commands/Invite.cpp \
			src/commands/Topic.cpp \
			src/Client.cpp \
			src/CommandHandler.cpp \
			src/Replies.cpp \
			src/Server.cpp \
			src/channel.cpp \
			main.cpp \
		
HEADERS = 	inc/Client.hpp \
			inc/channel.hpp \
			inc/Command.hpp \
			inc/CommandHandler.hpp \
			inc/Replies.hpp \
			inc/Server.hpp \

OBJECT = $(addprefix ${PREFIX}, $(SOURCE:.cpp=.o))

color := $(shell tput setaf 2)$(shell tput bold)
reset := $(shell tput sgr0)

run: $(NAME)
	@echo "running with password: $(color)pass$(reset)"
	@./$(NAME) 9001 pass

all: run

${PREFIX}:
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}src
	mkdir -p ${PREFIX}src/commands

${NAME}: ${PREFIX} $(OBJECT) $(HEADERS)
	$(CC) $(CPPFLAGS) $(OBJECT) -o $(NAME)

${PREFIX}%.o: %.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf ${PREFIX}
	rm -rf ${PREFIX}src
	rm -rf ${PREFIX}src/commands
	rm -rf $(OBJECT)

fclean: clean
	rm -rf ${NAME}

re: fclean all

.PHONY: all clean fclean re


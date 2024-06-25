CPPFLAGS = -std=c++11
CC = g++
NAME = ircserv
PREFIX = ./obj/
ASCII = \n\
██╗██████╗  ██████╗███████╗███████╗██████╗ ██╗   ██╗\n\
██║██╔══██╗██╔════╝██╔════╝██╔════╝██╔══██╗██║   ██║\n\
██║██████╔╝██║     ███████╗█████╗  ██████╔╝██║   ██║\n\
██║██╔══██╗██║     ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝\n\
██║██║  ██║╚██████╗███████║███████╗██║  ██║ ╚████╔╝ \n\
╚═╝╚═╝  ╚═╝ ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝ \n\
Contributors: ridwane, makram, kaoutar

SOURCE = src/commands/Nick.cpp \
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
         main.cpp

HEADERS = inc/Client.hpp \
          inc/channel.hpp \
          inc/Command.hpp \
          inc/CommandHandler.hpp \
          inc/Replies.hpp \
          inc/Server.hpp

OBJECT = $(addprefix $(PREFIX), $(SOURCE:.cpp=.o))

COLOR = \033[1;32m
RESET = \033[0m
RED = \033[1;31m\033[1m

.PHONY: all run clean fclean re banner

all: banner run

banner:
	@echo "$(COLOR)$(ASCII)$(RESET)"

run: $(NAME)
	@echo "Running with password: $(RED)pass$(RESET)"
	@./$(NAME) 9001 pass

$(PREFIX):
	@mkdir -p $(PREFIX)src/commands
	@echo "Creating directories..."

$(NAME): $(PREFIX) $(OBJECT)
	@echo "Building $(NAME)..."
	@$(CC) $(CPPFLAGS) $(OBJECT) -o $(NAME)
	@echo "$(COLOR)Build complete!$(RESET)"

$(PREFIX)%.o: %.cpp $(HEADERS)
	@$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	@rm -rf $(PREFIX)
	@rm -f $(OBJECT)

fclean: clean
	@rm -f $(NAME)

re: fclean run
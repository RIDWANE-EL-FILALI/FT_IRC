#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

bool connectToServer(const std::string &server, int port, int &sockfd) {
    struct sockaddr_in server_addr;
    struct hostent *host;

    if ((host = gethostbyname(server.c_str())) == NULL) {
        std::cerr << "Failed to get host by name ❌" << std::endl;
        return false;
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Failed to create socket ❌" << std::endl;
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    memset(&(server_addr.sin_zero), '\0', 8);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        std::cerr << "Failed to connect to server ❌" << std::endl;
        close(sockfd);
        return false;
    }
    return true;
}

bool sendToServer(int sockfd, const std::string &message) {
    if (send(sockfd, message.c_str(), message.length(), 0) == -1) {
        std::cerr << "Failed to send message ❌" << std::endl;
        return false;
    }
    return true;
}

void listenForMessages(int sockfd) {
    char buffer[512];
    int numBytes;

    while (true) {
        if ((numBytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[numBytes] = '\0';
            std::cout << buffer;

            // Check if the message is a PING
            if (strncmp(buffer, "PING", 4) == 0) {
                // Respond with PONG
                std::string response = "PONG " + std::string(buffer + 5) + "\r\n";
                if (!sendToServer(sockfd, response)) {
                    std::cerr << "Failed to send PONG response ❌" << std::endl;
                    break;
                }
                std::cout << "Sent: " << response;
            }
        } else if (numBytes == 0) {
            std::cout << "Server closed the connection" << std::endl;
            break;
        } else {
            std::cerr << "Failed to receive message ❌" << std::endl;
            break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <server> <port> <password>" << std::endl;
        return 1;
    }

    std::string server = argv[1];
    int port = std::stoi(argv[2]);
    std::string password = argv[3];
    int sockfd;

    if (connectToServer(server, port, sockfd)) {
        std::cout << "Connected to server ✅" << std::endl;

        // Send registration messages
        if (!sendToServer(sockfd, "PASS " + password + "\r\n") ||
            !sendToServer(sockfd, "NICK bot\r\n") ||
            !sendToServer(sockfd, "USER botname 0 * :bot\r\n")) {
            std::cerr << "Failed to register with the server ❌" << std::endl;
            close(sockfd);
            return 1;
        }
        std::cout << "Registered with the server ✅" << std::endl;

        // Listen for incoming messages
        std::cout << "Listening for messages..." << std::endl;
        listenForMessages(sockfd);

        // Close the socket when done
        close(sockfd);
    } else {
        std::cerr << "Failed to connect to server ❌" << std::endl;
    }

    return 0;
}
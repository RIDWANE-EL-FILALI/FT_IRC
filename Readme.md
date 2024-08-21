![image](https://github.com/RIDWANE-EL-FILALI/FT_IRC/blob/master/images/Ingoude_Company.png)

# IRC Server in C++98

## Overview

This project is an implementation of an IRC (Internet Relay Chat) server using C++98. The server adheres to the IRC protocol and allows multiple clients to connect simultaneously, communicate, and perform basic IRC functions such as joining channels, sending private messages, and setting user modes. The project is designed to run on Unix-like systems and is specifically tailored for compatibility with macOS.

## Features

- **Multiple Client Handling**: The server can manage multiple clients at once without blocking or hanging.
- **Non-Blocking I/O**: All input/output operations are non-blocking, ensuring efficient resource usage and smooth operation.
- **TCP/IP Communication**: The server communicates with clients over TCP/IP (IPv4 or IPv6).
- **Basic IRC Functionality**:
  - User authentication
  - Nickname and username management
  - Joining channels
  - Sending and receiving private messages
  - Broadcasting messages to all users in a channel
- **Channel Operations**:
  - **KICK**: Eject a client from a channel
  - **INVITE**: Invite a client to a channel
  - **TOPIC**: Change or view the channel's topic
  - **MODE**: Modify channel modes including:
    - `i`: Invite-only mode
    - `t`: Restrict topic changes to operators
    - `k`: Set/remove channel key (password)
    - `o`: Manage operator privileges
    - `l`: Set/remove user limit in the channel
- **Operator and Regular Users**: The server distinguishes between operators and regular users, allowing operators to perform administrative tasks within channels.

## Additional Details

- **Platform Compatibility**: The project includes specific accommodations for macOS, ensuring that file descriptors are handled in a non-blocking manner similar to other Unix systems.
- **Testing**: The server has been rigorously tested to handle various scenarios, including partial data reception and low bandwidth situations.

## Bonus Features (Optional)

For enhanced functionality, the following features could be added:
- **File Transfer**: Enabling users to transfer files through the IRC server.
- **Bot Integration**: Implementing a bot to automate tasks or provide additional services.

# ⛓️LINKS:
* [My article on medium about IRC](https://medium.com/@ridwaneelfilali/internet-relay-chat-da58a0e4d2ba)

# 🥇Badge:
![image](https://github.com/RIDWANE-EL-FILALI/42_badges_1337/blob/master/ft_ircn.png)

# 📚Skills:
* Rigor
* Unix
* Network & system administration
* Object-oriented programming

# 📱REACH ME
Made by RIDWANE EL FILALI 👋 [See my linkedin](https://www.linkedin.com/in/ridwane-elfilali-0ab7aa253/)

<p align="center">
<h1 align="center"><strong>If you like this repo star it</strong></h1>
</p>

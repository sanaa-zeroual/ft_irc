# ft_irc

A fully functional **IRC server** built in C++98, developed as part of the 42 School curriculum. The server handles multiple simultaneous clients using non-blocking I/O and implements core IRC protocol commands.

## Features

- Multi-client support using `poll()` / non-blocking sockets
- Password-protected server connection
- User authentication (PASS, NICK, USER)
- Channel creation and management
- Operator privileges and channel modes
- Built-in IRC bot
- Compliant with the IRC protocol (RFC 1459)

## Supported Commands

| Command | Description |
|---------|-------------|
| `PASS` | Authenticate with the server password |
| `NICK` | Set or change nickname |
| `USER` | Register a user |
| `JOIN` | Join a channel |
| `PART` | Leave a channel |
| `PRIVMSG` | Send a message to a user or channel |
| `KICK` | Remove a user from a channel (operator) |
| `INVITE` | Invite a user to a channel |
| `TOPIC` | Set or view the channel topic |
| `MODE` | Set channel/user modes |
| `QUIT` | Disconnect from the server |

## Project Structure

```
ft_irc/
├── include/
│   └── Server.hpp         # Class declarations
├── sources/
│   ├── Server.cpp         # Server setup and event loop
│   ├── Client.cpp         # Client connection handling
│   ├── Commands.cpp       # IRC command parsing and execution
│   ├── Channel.cpp        # Channel management
│   ├── Utils.cpp          # Utility functions
│   └── bot.cpp            # Built-in bot
├── main.cpp
└── Makefile
```

## Build & Run

```bash
# Compile
make

# Start the server
./server <password> <port>

# Example
./server mypassword 6667
```

## Connect with an IRC Client

You can connect using any IRC client (e.g., **irssi**, **WeeChat**, **HexChat**):

```bash
# Using netcat for quick testing
nc localhost 6667

# Then authenticate
PASS mypassword
NICK yournick
USER youruser 0 * :Real Name
```

## Requirements

- C++98 compiler (`g++` or `clang++`)
- Linux / macOS

## Clean

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full rebuild
```

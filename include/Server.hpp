#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include "Client.hpp"
#include <algorithm>
#include "Commands.hpp"

#define MAX_CLIENTS 100

class IRCServer {
private:
    std::string password;   
    int port;
    int nfds;
    int server_fd;
    
    struct sockaddr_in address;
    std::vector<Client *> clients;
    std::map<std::string, Channel*> channels;
    struct pollfd fds[MAX_CLIENTS];

    std::map<int, std::string> clientBuffers;
    std::map<int, std::string> sendBuffers;

    Client* getClientByFd(int fd);

public:
std::map<std::string, Channel*>& getChannels() { return channels; }
    IRCServer(const std::string &pass, int p);
    IRCServer();
    ~IRCServer();
    bool setupServer();
    void run();
    std::string CheckCommand(int fd);
    void ExecuteCommand(int fd);


private:
    void acceptClient();
    void handleClient(int index);
};

#endif

// #ifndef SERVER_HPP
// #define SERVER_HPP

// #include <iostream>
// #include <string>
// #include <vector>
// #include <map>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <poll.h>
// #include "Client.hpp"

// #define MAX_CLIENTS 100

// class IRCServer {
// private:
//     int server_fd;
//     struct sockaddr_in address;
//     std::vector<Client *> clients;
//     struct pollfd fds[MAX_CLIENTS];
//     int nfds;

//     std::map<int, std::string> clientBuffers;
//     std::map<int, std::string> sendBuffers;

//     std::string password;   
//     int port;
//     Client* getClientByFd(int fd);

// public:
//     IRCServer(const std::string &pass, int p);
//     IRCServer();
//     bool setupServer();
//     void run();
//     std::string CheckCommand(int fd);
//     void ExecuteCommand(int fd, std::string cmd);
// private:
//     void acceptClient();
//     void handleClient(int index);
// };

// #endif



// #ifndef SERVER_HPP
// #define SERVER_HPP

// #include <iostream>
// #include <string>
// #include <vector>
// #include <map>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include <poll.h>
// #include "Client.hpp"
// #define PORT 6667
// #define MAX_CLIENTS 100

// class IRCServer {
// private:
//     int server_fd;
//     struct sockaddr_in address;
//     std::vector<Client> clients;
//     struct pollfd fds[MAX_CLIENTS];
//     int nfds;

//     std::map<int, std::string> clientBuffers;
//     std::map<int, std::string> sendBuffers;

//     std::string password;   
//     int port;              

// public:
//     // IRCServer(const std::string &pass, int p);
//     IRCServer();
//     bool setupServer();
//     void run();
//     std::string CheckCommand(int fd);
//     void ExecuteCommand(int fd);
// private:
//     void acceptClient();
//     void handleClient(int index);
// };

// #endif
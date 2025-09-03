#include "../include/Server.hpp"
#include "../include/Client.hpp"
#include <stdexcept>
#include <cstring>

IRCServer::IRCServer() : nfds(0) , server_fd(-1) {}

IRCServer:: IRCServer(const std::string &pass, int p):password(pass), port(p){
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    }
}

bool IRCServer::setupServer() {
    int opt = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        std::cerr << "error while creating a socket " << std::endl; 

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
         std::cerr << "error sockopt" << std::endl; 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << "bind error" << std::endl; 
        std::cout << "the address " << &address << std::endl;
        std::cout << "the size of the address " << sizeof(address) << std::endl; 
        std::cout << "the  ip type "<< address.sin_family << std::endl;
        std::cout << "which address " << address.sin_addr.s_addr << std::endl;
        return false;

    }
    std::cout <<  "the port " << port << std::endl;
    if (listen(server_fd, SOMAXCONN) < 0)
        std::cerr << "listen error " << std::endl; 

    std::cout << "IRC server listening on port " << port << std::endl;
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    nfds = 1;
    return true;
}

void IRCServer::acceptClient()
{
    int new_fd = accept(server_fd, NULL, NULL);
    if (new_fd < 0)
    {
        std::cerr << "accept error " << std::endl; 
        return ;
    }

    if (nfds >= MAX_CLIENTS)
    {
        std::cerr << "Max clients reached" << std::endl;
        close(new_fd);
        return;
    }
    std::cout << "New client connected: fd " << new_fd << std::endl;
    Client* newClient = new Client(new_fd);
    clients.push_back(newClient);           
    
    clientBuffers[new_fd] = "";             
    sendBuffers[new_fd] = "";
    
    fds[nfds].fd = new_fd;
    fds[nfds].events = POLLIN;
    nfds++;
}

// void IRCServer::handleClient(int index)
// {
//     int fd = fds[index].fd;
//     // char buf[1024];

//     std::string buf(1024, '\0');
//     int valread = recv(fd, &buf[0], buf.size() - 1, 0);
//     if (valread <= 0) {
//         std::cout << "Client disconnected: fd " << fd << std::endl;
//         close(fd);
//         fds[index] = fds[nfds - 1];
//         nfds--;
//         clientBuffers.erase(fd);
//         sendBuffers.erase(fd);
//         clients.erase(clients.begin() + index - 1);
//     }
//     // if(!clientBuffers[fd].empty())
//     else {
//         clientBuffers[fd] += std::string(buf, valread); 
//         std::cout << "Received from fd " << fd << ": " << clientBuffers[fd];
//         std::cout << "the client buffer is not empty:" << "{" << clientBuffers[fd] << "}" << std::endl;
//         sendBuffers[fd] = clientBuffers[fd];
//         send(fd, sendBuffers[fd].c_str(), sendBuffers[fd].size(), 0);
//         std::string cmd = CheckCommand(fd);
//         while (!cmd.empty()) {
//             std::cout << "Execute command: " << cmd << std::endl;
//             ExecuteCommand(fd);
//         }
//     }
// }

void IRCServer::handleClient(int index)
{
    int fd = fds[index].fd;
    char buf[1024];

    int valread = recv(fd, buf, sizeof(buf) - 1, 0);
    if (valread <= 0) {
        std::cout << "Client disconnected: fd " << fd << std::endl;
        close(fd);

        Client* client = getClientByFd(fd);
        if (client) {
            std::vector<Client*>::iterator it = std::find(clients.begin(), clients.end(), client);
            if (it != clients.end()) {
                clients.erase(it);
                delete client;
            }
        }

        fds[index] = fds[nfds - 1];
        nfds--;
        clientBuffers.erase(fd);
        sendBuffers.erase(fd);
        return;
    }
    buf[valread] = '\0';

    clientBuffers[fd].append(buf, valread);

    std::string cmd;
    Client* client = getClientByFd(fd);
    while (client && !(cmd = CheckCommand(fd)).empty()) {
        std::cout << "cmd: {" << cmd << "}" << std::endl;
        handleCommand(*client, cmd, password, clients, channels);
    }
}

// std::string IRCServer::CheckCommand(int fd) {
//     size_t pos = clientBuffers[fd].find("\r\n");
//     std::string cmd;
//     if (pos != std::string::npos) {
//          cmd = clientBuffers[fd].substr(0, pos);
//         clientBuffers[fd].erase(0, pos + 2); 
//         return cmd;
//     }
//     return "";
// }
std::string IRCServer::CheckCommand(int fd) {
    std::string &buffer = clientBuffers[fd];

    size_t pos = buffer.find("\r\n");
    size_t len = 2;

    if (pos == std::string::npos) {
        pos = buffer.find("\n");
        len = 1;
    }

    if (pos != std::string::npos) {
        std::string cmd = buffer.substr(0, pos);
        buffer.erase(0, pos + len);
        return cmd;
    }

    return "";
}


void IRCServer::ExecuteCommand(int fd) {
    std::cout << "execute command " << fd << ": " << std::endl;
}

void IRCServer::run() {
    while (true) {
        int activity = poll(fds, nfds, -1);
        if (activity < 0) 
            std::cerr << "poll error " << std::endl; 

        if (fds[0].revents & POLLIN) 
            acceptClient();
        for (int i = 1; i < nfds; i++) {
            if (fds[i].revents & POLLIN){
                 handleClient(i);
                }
                }
    }
    close(server_fd);
}

Client* IRCServer::getClientByFd(int fd)
{
    for (size_t i = 0; i < clients.size(); i++) {
        if (clients[i]->getFd() == fd)
            return clients[i];
    }
    return NULL;
}

IRCServer::~IRCServer()
{
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it) {
        delete it->second;
    }

    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
        delete *it;
    }
    clients.clear();
}






// #include <stdexcept>
// #include <cstring>

// IRCServer::IRCServer() : nfds(0) {}
// // IRCServer :: IRCServer(const std::string &pass, int p): pass(password), p(port){}
// bool IRCServer::setupServer() {
//     int opt = 1;
//     server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd < 0)
//         std::cerr << "error while creating a socket " << std::endl; 

//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
//          std::cerr << "error sockopt" << std::endl; 
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
//          std::cerr << "bind error" << std::endl;
//     }
//     if (listen(server_fd, SOMAXCONN) < 0)
//          std::cerr << "listen error " << std::endl; 
//     std::cout << "IRC server listening on port " << PORT << std::endl;
//     fds[0].fd = server_fd;
//     fds[0].events = POLLIN;
//     nfds = 1;
//     return true;
// }

// void IRCServer::acceptClient() {
//     int new_fd = accept(server_fd, NULL, NULL);
//     if (new_fd < 0)
//           std::cerr << "accept error " << std::endl; 

//     if (nfds >= MAX_CLIENTS) {
//         std::cerr << "Max clients reached" << std::endl;
//         close(new_fd);
//         return;
//     }
//     std::cout << "New client connected: fd " << new_fd << std::endl;
//     clients.push_back(new_fd);           
//     clientBuffers[new_fd] = "";             
//     sendBuffers[new_fd] = "";
//     fds[nfds].fd = new_fd;
//     fds[nfds].events = POLLIN;
//     nfds++;
// }

// void IRCServer::handleClient(int index) {
//     int fd = fds[index].fd;
//     char buf[1024];
//     int valread = recv(fd, buf, sizeof(buf), 0);
//     if (valread <= 0) {
//         std::cout << "Client disconnected: fd " << fd << std::endl;
//         close(fd);
//         fds[index] = fds[nfds - 1];
//         nfds--;
//         clientBuffers.erase(fd);
//         sendBuffers.erase(fd);
//         clients.erase(clients.begin() + index - 1);
//     }
//     else {
//         clientBuffers[fd] += std::string(buf, valread);
//         std::cout << "valread1: " << valread << std::endl;
//         std::cout << "Received from fd " << fd << ": {" << clientBuffers[fd] << "}" << std::endl;
//         std::string cmd;
//         while (!(cmd = CheckCommand(fd)).empty()) {
//             std::cout << "Execute command: " << cmd << std::endl;
//             ExecuteCommand(fd);
//         }
//     }
// }

// std::string IRCServer::CheckCommand(int fd) {
//     size_t pos = clientBuffers[fd].find("\r\n");
//     std::string cmd;
//     if (pos != std::string::npos) {
//          cmd = clientBuffers[fd].substr(0, pos);
//         clientBuffers[fd].erase(0, pos + 2); 
//         std::cout << "cmd: " << cmd << std::endl;
//         return cmd;
//     }
//     return "";
// }

// void IRCServer::ExecuteCommand(int fd) {
//     std::cout << "execute command " << fd << ": " << std::endl;
// }

// void IRCServer::run() {
//     while (true) {
//         int activity = poll(fds, nfds, -1);
//         if (activity < 0) 
//             std::cerr << "poll error " << std::endl; 

//         if (fds[0].revents & POLLIN) 
//             acceptClient();
//         for (int i = 1; i < nfds; i++) {
//             if (fds[i].revents & POLLIN){
//                  handleClient(i);
//                 }
//                 }
//     }
//     close(server_fd);
// }
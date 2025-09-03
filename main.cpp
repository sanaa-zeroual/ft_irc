#include "include/Server.hpp"
#include <cstdlib>   // for atoi

int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "Use " << av[0] << " <password> <port>" << std::endl;
        return 1;
    }
    std::string password = av[1];
    int port = atoi(av[2]);  
    IRCServer server(password, port);
    if (!server.setupServer()) 
        return 1;
    server.run();
    return 0;
}


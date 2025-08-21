#include "include/Client.hpp"
#include "include/Commands.hpp"


int main()
{
    //must parse line first
    std::string password = "test";
    std::string random_str = "PASS test";
    std::string random_str2 = "NICK      hello";

    Client client(5);

    handleCommand(client, random_str, password);
    handleCommand(client, random_str2, password);

    std::cout << "Authenticated? " << client.isAuthenticated() << std::endl;
    std::cout << "Nickname: " << client.getNick() << std::endl;

    std::cout << "yes " << true << std::endl;
    std::cout << "no " << false << std::endl;

}



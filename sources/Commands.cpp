#include "../include/Client.hpp"

void handlePass(Client &client, const std::string &line, const std::string &sreverPass)
{
    std::string recievPass = line.substr(5);
    
    if (recievPass == sreverPass)
    {
        client.setAuthenticated(true);
        std::cout << "Client fd " << client.getFd() << " authenticated successfully." << std::endl;
    }
    else
    {
        client.setAuthenticated(false);
        std::cout << "Client fd " << client.getFd() << " authentication failed. disconection" << std::endl;
    }
}

void handleNick(Client &client, const std::string &line)
{
    std::string nick = line.substr(5);
    if (nick.empty())
    {
        std::cout << "Client fd " << client.getFd() << " sent an empty name" << std::endl;
        return ;
    }
    client.setNick(nick);
    std::cout << "Client fd " << client.getFd() << " set nickname: " << nick << std::endl;
}


void handleCommand(Client &client, const std::string &line, const std::string &serverPass)
{
    std::cout << line << std::endl;
    std::cout << "before {" << line.substr(0, 5) << "}" << std::endl;
    if (line.substr(0, 4) == "PASS")
    {
        // std::cout << line.substr(0, 5) << std::endl;
        handlePass(client, line, serverPass);
    }
    else if (line.substr(0, 4) == "NICK")
    {
        handleNick(client, line);
    }
    else
        std::cout << "unknown command" << std::endl; 
}

// std::string parse(std::string &line)
// {
    
// }

void handleUserNmae(Client &client, const std::string &line)
{

}


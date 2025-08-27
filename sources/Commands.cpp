#include "../include/Commands.hpp"

//must check the authentication
void handlePass(Client &client, const std::string &line, const std::string &sreverPass)
{
    std::string recievPass = extractString(line, 2);
    std::string msg;

    if (client.isAuthenticated())
    {
        std::string msg = ":irc.server 462 " + client.getNick() + " :You already registred\r\n";
        client.getBuffer() += msg;
        std::cout << "already registred: " << client.isAuthenticated() << std::endl;//for debug
        return ;
    }
    if (recievPass.empty())
    {
        std::string msg = ":irc.server 461 PASS :Not enough parameters\r\n";
        client.getBuffer() += msg;
        std::cout << "empty password: " << client.isAuthenticated() << std::endl;
        return;
    }
    if (recievPass == sreverPass)
    {
        client.setAuthenticated(true);
        std::string msg = ":irc.server NOTICE " + client.getNick() + 
                          " :Password accepted\r\n";
        client.getBuffer() += msg;
        std::cout << "Client fd " << client.getFd() << " authenticated successfully.: "  << client.isAuthenticated() << std::endl;//for debug
    }
    else
    {
        client.setAuthenticated(false);
        std::string msg = ":irc.server NOTICE " + client.getNick() + 
                          " :Password incorrect\r\n";
        client.getBuffer() += msg;

        std::cout << "Client fd " << client.getFd() << " authentication failed." << client.isAuthenticated() << std::endl;//for debug
    }
}

bool nick_exists(const std::string &nick, const std::vector<Client*> &clients)
{
    size_t i = 0;
    // while (i < clients.size())
    // {
    //     std::cout << "duplicate check: "<< i << " " << clients[i]->getNick() << std::endl;
    //     i++;
    // }
    // i = 0;
    while (i < clients.size())
    {
        if (clients[i]->getNick() == nick)
            return (true);
        i++;
    }
    return (false);
}




void check_conncetivity(Client &client)
{
    if (client.isRegistred())
    {
        std::string msg = ":irc.server 001 " + client.getNick() + " :Welcome to the IRC server!\r\n";
        client.getBuffer() += msg;
        std::cout << client.getFd() << "-nickname is: |" << client.getNick() << "| and " << client.getName() << "| registraion done" << std::endl;
        
        // client.send_welcome(client);
    }
}

void handleNick(Client &client, const std::string &line, const std::vector<Client*> &clients)
{
    std::string msg;

    std::string nick = extractString(line, 0);
    if (nick.empty())
    {
        msg = ":irc.server 431 * :No nickname given\r\n";
        client.getBuffer() += msg;
        std::cout << "empty nickname" << std::endl; // just for debug
        return ;
    }
    else if (nick_exists(nick, clients))
    {
        std::string msg = ":irc.server 433 * " + nick + " :Nickname is already in use\r\n";
        client.getBuffer() += msg;
        std::cout << "nickname already in use" << std::endl; // just for debug
        return;
    }
    client.setNick(nick);
    std::cout << "nickname is " << nick << std::endl;
    // if (client.isRegistred())
    // {
    //     // printf("check\n");
    //     check_conncetivity(client);
    // }
}



void handleCommand(Client &client, const std::string &line, const std::string &serverPass, const std::vector<Client*> &clients)
{
    std::string command = extractCommand(line);
    // std::cout << line << std::endl;
    // std::cout << "before {" << line.substr(0, 5) << "}" << std::endl;
    if (command == "PASS")
    {
        // std::cout << line.substr(0, 5) << std::endl;
        std::cout << "pass command" << std::endl;//just for debug
        handlePass(client, line, serverPass);
    }
    else if (command == "NICK" && client.isAuthenticated())
    {
        std::cout << "nick command" << std::endl;//just for debug
        handleNick(client, line, clients);
    }
    else if (command == "USER" && client.isAuthenticated())
    {
        std::cout << "user command" << std::endl;//just for debug
        handleUser(client, line);
    }
    else
    {
        if(client.isAuthenticated())
        {
            std::cout << "unknown command: for registration nick name and user name are mendatroy" << std::endl;//just for debug
            std::string msg = "unknown command: for registration nick name and user name are mendatroy\r\n";
            client.getBuffer() += msg;
        }
        else
        {
            std::cout << "unknown command: for registration enter PASS" << std::endl;//just for debug
            std::string msg = "unknown command: for registration nick name and user name are mendatroy\r\n";
            client.getBuffer() += msg;
        }
    }
    if (client.checkRegistration(client))
    {
        handleJoin();
    }
    
}


void handleUser(Client &client, const std::string &line)
{
    std::string name = extractString(line, 1);
    std::string msg;

    if (name.empty())
    {
        msg = ":irc.server 431 * :No username given\r\n";
        client.getBuffer() += msg;
        std::cout << "Client fd " << client.getFd() << " sent empty USER command.\n"; //just for debug
        return;
    }
    if (!client.getName().empty())
    {
        printf("username already registred\n");//just fro debug
        std::string msg = ":irc.server 462 " + client.getNick() + " :You may not reregister\r\n";
        client.getBuffer() += msg;
        // check_conncetivity(client);
        return;
    }
    client.setName(name);
    std::cout << "Client fd " << client.getFd() << " set username: " << name << "\n"; //just for debug
    // if (client.isRegistred())
    // {
    //     // printf("enter\n");
    //     check_conncetivity(client);
    // }
}
/*
    JOIN <channel>{,<channel>} [<key>{,<key>}]
    a channel start with # or &
    you can have multiple channels, one after one
    and optionnal words that are password for the channels
    must handle the password, ban, invite only
*/




void handlejoin(Client &client, const std::string &line, std::map<std::string, channel*> &channels)
{
    std::string msg;
    if (!client.isRegistred())
    {
        msg = ":irc.server 451 <nick> :You have not registered";
        client.getBuffer() += msg;
        std::cout << "you can't call join while you'r not registred" << std::endl;
    }
    
}

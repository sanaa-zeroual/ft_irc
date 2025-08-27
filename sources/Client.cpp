#include "../include/Client.hpp"

Client::Client(int fd_) : fd(fd_), authenticated(false), nickname(""), isconnected(false)
{
    this->connectionTime = time(NULL);
}

Client::~Client()
{

}

int Client::getFd() const
{
    return (this->fd);
}

std::string Client::getBuffer(){
    return (this->buffer);
}

bool Client::isAuthenticated() const
{
    return (this->authenticated);
}

bool Client::isConnected() const
{
    return (this->isconnected);
}

void Client::setConnected(bool val)
{
    this->isconnected = val;
}

void Client::setAuthenticated(bool val)
{
    this->authenticated = val;
}

void Client::setNick(std::string name)
{
    this->nickname = name;
}
std::string Client::getNick() const
{
    return (this->nickname);
}
bool Client::isRegistred() const 
{
    std::string msg;
    if (this->getNick().empty())
    {
        msg = "nick name needed for registration\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    if (this->getName().empty())
    {
        msg = "username needed for registration\r\n";
        send(fd, msg.c_str(), msg.size(), 0);
    }
    return (!this->getNick().empty() && !this->getName().empty() && this->authenticated);
}
void Client::setName(std::string &user_name)
{
    this->username = user_name;
}

std::string Client::getName() const
{
    return (this->username);
}

void Client::send_welcome(Client &client)
{
    int fd = client.getFd();
    std::string nick = client.getNick();

    std::string msg = ":irc.server " + nick +
        " :Welcome to the IRC network, " + nick + "\r\n";
    send(fd, msg.c_str(), msg.size(), 0);

    // msg = ":irc.server " + nick +
    //     " irc.server o i t k l o\r\n";
    // send(fd, msg.c_str(), msg.size(), 0);
}

time_t Client::getConnectionTime() const
{
    return (this->connectionTime);
}

bool Client::checkRegistration(Client &client)
{
    if (client.isRegistred())
    {
        std::cout << "Client fd " << getFd() << " is fully registered.\n";
        // client.setConnected(true);
        return true;
    }
    else
    {
        time_t now = time(NULL);
        if (difftime(now, getConnectionTime()) > 600)
        {
            std::string msg = ":irc.server NOTICE * :Registration timeout renter the password\r\n";
            getBuffer() += msg;
            std::cout << "Client fd " << getFd() << " failed to register in 10 min. Disconnecting...\n"; //just for debug
            // close(getFd());// if diconnect handled
            return (false);
        }
    }
}

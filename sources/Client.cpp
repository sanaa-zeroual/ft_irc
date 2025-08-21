#include "../include/Client.hpp"

Client::Client(int fd_) : fd(fd_), authenticated(false)
{}

Client::~Client()
{

}

int Client::getFd() const
{
    return (this->fd);
}

bool Client::isAuthenticated() const
{
    return (this->authenticated);
}

void Client::setAuthenticated(bool val)
{
    this->authenticated = val;
}

void Client::setNick(std::string &name)
{
    this->nickname = name;
}
std::string Client::getNick()
{
    return (this->nickname);
}
void Client::setName(std::string &user_name)
{
    this->username = user_name;
}

std::string Client::getName()
{
    return (this->username);
}
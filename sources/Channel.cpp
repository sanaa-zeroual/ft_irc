#include "../include/Channel.hpp"

Channel::Channel(const std::string &name) : name(name), key("")
{
}

const std::string &Channel::getName() const
{
    return (this->name);
}


const std::string &Channel::getKey() const
{
    return (this->key);
}

const std::vector<Client*> &Channel::getClients() const
{
    return (this->Clients);
}

void Channel::addClient(Client *client)
{
    this->Clients.push_back(client);
}


bool Channel::hasClient(Client *client) const
{
    std::vector<Client*>::const_iterator it = Clients.begin();
    while (it != Clients.end())
    {
        if (*it == client)
            return true;
        ++it;
    }
    return (false);
}
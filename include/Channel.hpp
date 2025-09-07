#ifndef CHANNEL_HPP
#define CHANNEL_HPP


#include <string>
#include <vector>
#include "Client.hpp"

class Channel
{
    private:
        std::string name;
        std::string key;
        std::vector<Client*> Clients;
    public:
        Channel(const std::string &name);

         const std::string &getName() const;
        const std::string &getKey() const;
        const std::vector<Client*> &getClients() const;
        bool isMember(Client &client);
    void addClient(Client *client);
    void removeClient(Client *client);
        bool hasClient(Client *client) const;
};

#endif
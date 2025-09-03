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

        void addClient(Client *client);
        bool hasClient(Client *client) const;
};

#endif
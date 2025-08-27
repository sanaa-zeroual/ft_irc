#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sys/socket.h>


class Client
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        std::string buffer;
        bool authenticated;
        bool isconnected;
        time_t connectionTime;

    public:
        Client(int fd_);
        ~Client();

        int getFd() const;
        std::string getBuffer();
        //PASS
        bool isAuthenticated() const;
        void setAuthenticated(bool val);
        bool isConnected() const;
        void setConnected(bool val);
        //NICK NAME
        void setNick(std::string name);
        std::string getNick() const;
        bool isRegistred() const;
        //user name
        void setName(std::string &user_name);
        std::string getName() const;

        void send_welcome(Client &client);
        bool checkRegistration(Client &client);
        time_t getConnectionTime() const;

};


#endif

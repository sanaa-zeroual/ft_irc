#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

class Client
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        bool authenticated;
    public:
        Client(int fd_);
        ~Client();

        int getFd() const;
        //PASS
        bool isAuthenticated() const;
        void setAuthenticated(bool val);
        //NICK NAME
        void setNick(std::string &name);
        std::string getNick();
        //user name
        void setName(std::string &user_name);
        std::string getName();
};


#endif

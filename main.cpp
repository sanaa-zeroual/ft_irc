#include "include/Client.hpp"
#include "include/Commands.hpp"


int main()
{
    //must parse line first
    std::string password = "test";

    std::string random_st1_ = "PASS test dwfr ght5 ";

    std::string random_st1 = "PASS test dwfr ght5 ";
    std::string random_st2 = "PASS           ddtest";
    std::string random_st3 = "       PASS            test        ";
    std::string random_st4 = "PASS   testt ";
    std::string random_st5 = "PASS   ";

    std::string random_str1_ = "            NICK             hello1                   test               shwj";
    
    std::string random_str1 = "            NICK             hello1                   test               shwj";
    std::string random_str2 = "NICK hello2";
    std::string random_str3 = "NICK hello2";
    std::string random_str4 = "    ";
    std::string random_str5 = "NICK hello5";
    
    std::string random_str11_ = "USER katty";

    std::string random_str11 = "USER katty";
    std::string random_str22 = "USER         fifa lina      def   ";
    std::string random_str33 = "USER hhhhhhhhhhhhhhhhh de ";
    std::string random_str44 = "    ";
    std::string random_str55 = "USER kkikiki ksw";

    std::vector<Client*> clients;
    Client client1(1);
    Client client2(2);
    Client client3(3);
    Client client4(4);
    Client client5(5);

    // std::vector<Client*> clients_;
    // Client client1_(1);
    // clients_.push_back(&client1_);
    // clients.push_back(&client2);
    // clients.push_back(&client3);
    // clients.push_back(&client4);
    // clients.push_back(&client5);

    // handlePass(client1, random_st1, password);
    // handlePass(client2, random_st2, password);
    // handlePass(client3, random_st3, password);
    // handlePass(client1, random_st4, password);
    // handlePass(client4, random_st5, password);


    // handleNick(client1, random_str1, clients);
    // handleNick(client2, random_str2, clients);
    // handleNick(client3, random_str3, clients);
    // handleNick(client4, random_str4, clients);
    // handleNick(client1, random_str5, clients);

    // handleUser(client1, random_str11);
    // handleUser(client2, random_str22);
    // handleUser(client3, random_str33);
    // handleUser(client4, random_str44);
    // handleUser(client1, random_str55);
    // std::cout << "-----------------------------------" << std::endl;
    // handleCommand(client1_, random_st1_, password, clients_);
    // handleCommand(client1_, random_str1_, password, clients_);
    // handleCommand(client1_, random_str11_, password, clients_);
    std::cout << "-----------------------------------" << std::endl;

    handleCommand(client1, random_st1, password, clients);
    handleCommand(client1, random_str1, password, clients);
    handleCommand(client1, random_str11, password, clients);
    std::cout << "-----------------------------------" << std::endl;
    handleCommand(client1, random_st1, password, clients);
    handleCommand(client1, random_str1, password, clients);
    handleCommand(client1, random_str11, password, clients);
    std::cout << "-----------------------------------" << std::endl;

    handleCommand(client2, random_st2, password, clients);
    handleCommand(client2, random_str2, password, clients);
    handleCommand(client2, random_str22, password, clients);
    std::cout << "-----------------------------------" << std::endl;

    handleCommand(client3, random_st3, password, clients);
    handleCommand(client3, random_str3, password, clients);
    handleCommand(client3, random_str33, password, clients);

}

//if no password dont go to nick or user

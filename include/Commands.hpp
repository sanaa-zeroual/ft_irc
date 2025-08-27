#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Utils.hpp"
#include <vector>
#include <string>
#include <map>

void handleCommand(Client &client, const std::string &line, const std::string &serverPass, const std::vector<Client*> &clients);
// void handleCommand(Client &client, const std::string &line, const std::string &serverPass);
void handlePass(Client &client, const std::string &line, const std::string &sreverPass);
void handleNick(Client &client, const std::string &line, const std::vector<Client*> &clients);
void handleUser(Client &client, const std::string &line);

#endif
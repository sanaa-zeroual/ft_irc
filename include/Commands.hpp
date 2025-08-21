#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"

void handleCommand(Client &client, const std::string &line, const std::string &serverPass);
void handlePass(Client &client, const std::string &line, const std::string &sreverPass);
void handleNick(Client &client, const std::string &line);

#endif
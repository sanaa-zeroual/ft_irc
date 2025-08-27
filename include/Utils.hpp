#ifndef UTILS_COMMANDS_HPP
#define UTILS_COMMANDS_HPP

#include <string>
#include <cstddef>

std::string extractString(const std::string &line, int check);
std::string extractCommand(const std::string &line);

#endif
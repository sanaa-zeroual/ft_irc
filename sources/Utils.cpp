#include "../include/Utils.hpp"
#include <iostream>
#include <map>

std::string extractString(const std::string &line, int check)
{
    std::string nick = line;
    nick.erase(0, nick.find_first_not_of(" \t\r\n"));
    if (nick.substr(0, 4) != "NICK" && !check)
        return "";
    else if (nick.substr(0, 4) != "USER" && check == 1)
        return "";
    else if (nick.substr(0, 4) != "PASS" && check == 2)
        return "";
    if (nick.size() > 4)
        nick = nick.substr(4);
    else
        return "";

    nick.erase(0, nick.find_first_not_of(" \t\r\n"));

    size_t spacePos = nick.find(' ');
    if (spacePos != std::string::npos)
        nick = nick.substr(0, spacePos);

    nick.erase(nick.find_last_not_of(" \t\r\n") + 1);

    return nick;
}
std::string extractCommand(const std::string &line)
{
    std::string trimmed = line;
    // remove leading spaces/tabs/newlines
    trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));

    // find the first space
    size_t spacePos = trimmed.find(' ');
    std::string cmd;

    if (spacePos != std::string::npos)
        cmd = trimmed.substr(0, spacePos); // command is before the first space
    else
        cmd = trimmed; // whole line is the command if no space

    // uppercase for consistency (optional)
    for (size_t i = 0; i < cmd.size(); i++)
        cmd[i] = toupper(cmd[i]);

    return cmd;
}

// void parseCommandChannel(std::string &line, std::map<std::string, std::string> &channels)
// {
//     size_t spacePos = line.find(' ');
//     std::string ChannelPart = ()

// }



// int  main()
// {
//     std::cout << "|" << extractNick("              NICK     test   hhhh dewdw ") << "|" << std::endl;
// }
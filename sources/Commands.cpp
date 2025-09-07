#include "../include/Commands.hpp"

void handlePass(Client &client, const std::string &line, const std::string &sreverPass)
{
    std::string msg;

    if (client.isAuthenticated())
    {
        msg = ":irc.server 462 " + client.getNick() + " :You already registred\r\n";
        client.sendMsg(msg);
        return ;
    }
    if (line.empty())
    {
        msg = ":irc.server 461 PASS :Not enough parameters\r\n";
        client.sendMsg(msg);
        return;
    }
    if (line == sreverPass)
    {
        client.setAuthenticated(true);
        msg = ":irc.server NOTICE " + client.getNick() + " :Password accepted\r\n";
        client.sendMsg(msg);
    }
    else
    {
        client.setAuthenticated(false);
        msg = ":irc.server NOTICE " + client.getNick() + " :Password incorrect\r\n";
        client.sendMsg(msg);
    }
}

void handleNick(Client &client, const std::string &line, const std::vector<Client*> &clients)
{
    std::string msg;

    if (line.empty())
    {
        msg = ":irc.server 431 * :No nickname given\r\n";
        client.sendMsg(msg);
        return ;
    }
    else if (nick_exists(line, clients))
    {
        std::string msg = ":irc.server 433 * " + line + " :Nickname is already in use\r\n";
        client.sendMsg(msg);
        return;
    }
    client.setNick(line);
    check_conncetivity(client);
}

void handleUser(Client &client, const std::string &line)
{
    std::string name = line;
    std::string msg;

    if (name.empty())
    {
        msg = ":irc.server 431 * :No username given\r\n";
        client.sendMsg(msg);
        return;
    }
    if (!client.getName().empty())
    {
        std::string msg = ":irc.server 462 " + client.getNick() + " :USER name already registred\r\n";
        client.sendMsg(msg);
        return;
    }
    client.setName(name);
    check_conncetivity(client);
}


void handleJoin(Client &client, const std::string &line, std::map<std::string, Channel*> &channels)
{
    if (!client.isRegistred())
    {
        client.sendMsg(":irc.server 451 :You have not registered yet\r\n");
        return;
    }

    std::vector<std::pair<std::string, std::string> > joinVec = parseJoinVec(line, client);
    size_t i = 0;
    while (i < joinVec.size())
    {
        std::string channelName = joinVec[i].first;
        std::string key = joinVec[i].second;
        Channel* ch;
        if (channels.find(channelName) == channels.end())
        {
            ch = new Channel(channelName);
            channels[channelName] = ch;
        }
        else
            ch = channels[channelName];
        if (!ch->hasClient(&client))
        {
            ch->addClient(&client);
            client.addChannel(channelName);

            std::string joinMsg = ":" + client.getNick() + " JOIN " + channelName + "\r\n";
            size_t j = 0;
            while (j < ch->getClients().size())
            {
                ch->getClients()[j]->sendMsg(joinMsg);
                j++;
            }
            std::string namesMsg = ":irc.server 353 " + client.getNick() + " = " + channelName + " :";
            const std::vector<Client*>& clientsInChannel = ch->getClients();
            j = 0;
            while (j < clientsInChannel.size())
            {
                namesMsg += clientsInChannel[j]->getNick() + " ";
                j++;
            }
            namesMsg += "\r\n";
            client.sendMsg(namesMsg);

            client.sendMsg(":irc.server 366 " + client.getNick() + " " + channelName + " :User list complete\r\n");
        }
        i++;
    }
}

void handlePrivmsg(Client &sender, const std::string &line, const std::vector<Client*> &clients, std::map<std::string, Channel*> &channels)
{
    std::string reciev = extractPrivmsgTarget(line);
    std::string msg = extractPrivmsgText(line);

    if (reciev.empty())
    {
        sender.sendMsg(":irc.server 411 " + sender.getNick() + " :No recipient given\r\n");
        return ;
    }
    if (msg.empty())
    {
        sender.sendMsg(":irc.server 412 " + sender.getNick() + " :No text to send\r\n");
        return;
    }
    std::vector<std::string> targetList = split(reciev, ',');
    size_t i = 0;
    size_t j = 0;
    std::string target;
    std::string message;
    bool found = false;
    while (i < targetList.size())
    {
        target = trim(targetList[i]);
        Channel *ch;
        if (!target.empty() && (target[0] == '#' || target[0] == '&' || target[0] == '!' ))
        {
            if (channels.find(target) == channels.end())
            {
                sender.sendMsg(":irc.server 403 " + sender.getNick() + " " + target + " :No such channel\r\n");
                i++;
                continue;
            }
            ch = channels[target];
            message = ":" + sender.getNick() + " PRIVMSG " + target + " :" + msg + "\r\n";
            j = 0;
            while (j < ch->getClients().size())
            {
                if (ch->getClients()[j] != &sender)
                    ch->getClients()[j]->sendMsg(message);
                j++;
            }
        }
        else
        {
            j = 0;
            while (j < clients.size())
            {
                if (clients[j]->getNick() == target)
                {
                    message = ":" + sender.getNick() + " PRIVMSG " + target + " :" + msg + "\r\n";
                    clients[j]->sendMsg(message);
                    found = true;
                    break;
                }
                j++;
            }
            if (!found)
                sender.sendMsg(":irc.server 401 " + sender.getNick() + " " + target + " :No such nick\r\n");
        }
        i++;
    }
}

void handleInvite(Client &inviter, const std::string &line, std::vector<Client*> &clients, std::map<std::string, Channel*> &channels) {
    std::vector<std::string> tokens = split(line, ' ');
    if (tokens.size() < 2) {
        inviter.sendMsg(":irc.server 461 INVITE :Not enough parameters\r\n");
        return;
    }
    std::string targetNick = tokens[0];
    std::string channelName = tokens[1];

    if (channels.find(channelName) == channels.end()) {
        inviter.sendMsg(":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }
    Channel *ch = channels[channelName];

    if (!ch->hasClient(&inviter)) {
        inviter.sendMsg(":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }

    Client *target = NULL;
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getNick() == targetNick) {
            target = clients[i];
            break;
        }
    }
    if (!target) {
        inviter.sendMsg(":irc.server 401 " + targetNick + " :No such nick\r\n");
        return;
    }

    if (ch->hasClient(target)) {
        inviter.sendMsg(":irc.server 443 " + targetNick + " " + channelName + " :is already on channel\r\n");
        return;
    }

    std::string inviteMsg = ":" + inviter.getNick() + " INVITE " + targetNick + " :" + channelName + "\r\n";
    target->sendMsg(inviteMsg);

    inviter.sendMsg(":irc.server 341 " + inviter.getNick() + " " + targetNick + " " + channelName + "\r\n");
}

void handleCommand(Client &client, const std::string &line, const std::string &serverPass, const std::vector<Client*> &clients, std::map<std::string, Channel*> &channels)
{
    std::string command = extractCommand(line);
    std::string text = extractString(line);

    if (command == "PASS")
        handlePass(client, text, serverPass);
    else if (command == "NICK" && client.isAuthenticated())
        handleNick(client, text, clients);
    else if (command == "USER" && client.isAuthenticated())
        handleUser(client, text);
    else if(command == "KICK")
    {
        std::string kickLine = extractString(line);
        handleKick(client, kickLine, const_cast<std::vector<Client*>&>(clients), channels);
    }
    else if(command == "INVITE")
    {
        std::string inviteLine = extractString(line);
        handleInvite(client, inviteLine, const_cast<std::vector<Client*>&>(clients), channels);
    }
    else if (command == "SHOW")
    {
        std::string msg = "=== Connected Clients ===\r\n";
        for (std::vector<Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
        {
            Client *c = *it;
            msg += "Nick: " + c->getNick() + " | User: " + c->getName();

            const std::vector<std::string>& channels = c->getChannels();
            if (!channels.empty())
            {
                msg += " | Channels: ";
                for (size_t i = 0; i < channels.size(); i++)
                {
                    msg += channels[i];
                    if (i != channels.size() - 1)
                        msg += ", ";
                }
            }
            else
                msg += " | Channels: None";
            msg += "\r\n";
        }
        client.sendMsg(msg);
    }
    else if (client.isRegistred())
    {
        if (command == "JOIN")
        {
            std::string joinLine = extractJoinString(line);
            handleJoin(client, joinLine, channels);
        }
        else if (command == "PRIVMSG")
        {
            std::cout << "enter to private message" << std::endl;
            handlePrivmsg(client, line, clients, channels);
        }
        else
        {
            std::string msg = "unknown command\r\n";
            client.sendMsg(msg);
        }
    }
    else
    {
        if(client.isAuthenticated())
        {
            std::string msg = "unknown command: for registration nick name and user name are mendatroy\r\n";
            client.sendMsg(msg);
        }
        else
        {
            std::string msg = "unknown command: for registration enter PASS\r\n";
            client.sendMsg(msg);
        }

    }
}


void handleKick(Client &kicker, const std::string &line, std::vector<Client*> &clients, std::map<std::string, Channel*> &channels) {
    std::vector<std::string> tokens = split(line, ' ');
    if (tokens.size() < 2) {
        kicker.sendMsg(":irc.server 461 KICK :Not enough parameters\r\n");
        return;
    }
    std::string channelName = tokens[0];
    std::string targetNick = tokens[1];
    std::string comment = (tokens.size() > 2) ? line.substr(line.find(targetNick) + targetNick.length()) : "";
    if (channels.find(channelName) == channels.end()) {
        kicker.sendMsg(":irc.server 403 " + channelName + " :No such channel\r\n");
        return;
    }
    Channel *ch = channels[channelName];
    if (!ch->hasClient(&kicker)) {
        kicker.sendMsg(":irc.server 442 " + channelName + " :You're not on that channel\r\n");
        return;
    }
    Client *target = NULL;
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i]->getNick() == targetNick) {
            target = clients[i];
            break;
        }
    }
    if (!target || !ch->hasClient(target)) {
        kicker.sendMsg(":irc.server 441 " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }
    ch->removeClient(target);
    target->removeChannel(channelName);
    std::string kickMsg = ":" + kicker.getNick() + " KICK " + channelName + " " + targetNick + " :" + comment + "\r\n";
    const std::vector<Client*> &chClients = ch->getClients();
    for (size_t i = 0; i < chClients.size(); ++i) {
        chClients[i]->sendMsg(kickMsg);
    }
    target->sendMsg(kickMsg);
}



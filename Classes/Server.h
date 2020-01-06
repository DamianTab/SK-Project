//
// Created by dejmian on 05.01.2020.
//

#ifndef SK_PROJECT_SERVER_H
#define SK_PROJECT_SERVER_H

#include <string>
#include <map>
#include "SocketHandler.h"
#include "Client.h"

class Server : public SocketHandler {
private:
    // Map with user login and socket (FD)
    static std::map<std::string, Client *> usersMap;

    void createServerSocket(int argc, char **argv);

public:
    Server(int argc, char **argv);

    ~Server();

    void handleEvent(uint32_t events) override;

    void closeServer();

    static void addClient(Client *client);

    static void deleteClient(std::string login);

    // Getters and setters
    std::map<std::string, Client *> &getUsersMap();

    void setUsersMap(std::map<std::string, Client *> &usersMap);
};


#endif //SK_PROJECT_SERVER_H

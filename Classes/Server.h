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
    static std::map<std::string, Client *> clientsMap;

    void createServerSocket(int argc, char **argv);

    void clearClientsMap();

public:
    Server(int argc, char **argv);

    ~Server() override;

    void handleEvent(uint32_t events) override;

    void closeServer();

    static void addClientToMap(Client *client);

    static void deleteClientFromMap(std::string login);

    static bool isInsideClientMap(std::string login);

    static void sendToAllClients(char * buffer);

    // Getters and setters
    static std::map<std::string, Client *> &getClientsMap();

    static void setClientsMap(std::map<std::string, Client *> &usersMap);
};


#endif //SK_PROJECT_SERVER_H

//
// Created by dejmian on 23.01.2020.
//

#ifndef SK_PROJECT_GAME_H
#define SK_PROJECT_GAME_H


#include <vector>
#include "Client.h"

class Game {
private:
    char letter;

    static int round;

//    BECAUSE OF ROUND NUMBER - DO NOT NEED TO PLACE MUTEX ON THIS VARIABLE
    static std::vector<Client *> clientsRankingByTime;

    void clearClientsPoints(bool shouldClearTotalPointsAndRound = false);

    void drawLetter();

    void incrementRound();

    void removeInactiveClients();

    void calculateResults();

    void sendResults();
public:

    Game();

    ~Game();

    static Game* gameInstance;

    void run();

    static void pushClientToTimeRankingWhenPossible(Client * client);

    static int getRound();

    static void setRound(int _round);

    char getLetter() const;

    void setLetter(char letter);

    static std::vector<Client *> &getClientsRankingByTime();

    static void setClientsRankingByTime(std::vector<Client *> &clientsRankingByTime);

};


#endif //SK_PROJECT_GAME_H

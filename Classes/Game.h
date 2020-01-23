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

    static std::vector<Client *> clientsRankingByTime;

    void clearClientsPoints(bool shouldClearTotalPoints = false);

    void drawLetter();

public:

    Game();

    ~Game();

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

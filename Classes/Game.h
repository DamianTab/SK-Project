//
// Created by dejmian on 23.01.2020.
//

#ifndef SK_PROJECT_GAME_H
#define SK_PROJECT_GAME_H


class Game {
private:
    char letter;

    static int round;

    void clearClientsPoints();

    void drawLetter();

public:

    Game();

    ~Game();

    void run();

    static int getRound();

    static void setRound(int _round);
};


#endif //SK_PROJECT_GAME_H
//
// Created by dejmian on 23.01.2020.
//

#include <Utils/utils.h>
#include <thread>
#include "Game.h"
#include "Server.h"

int Game::round;

Game::Game() {
    clearClientsPoints();
    std::thread t1(&Game::run, this);
    t1.detach();
}

Game::~Game() {
    clearClientsPoints();

}

void Game::run() {
    round++;
    while(Server::getUsersMap().size() >= 2){
        sleep(SLEEP_TIME);
        round++;
    }
    delete this;
}


// Getters and Setters
int Game::getRound() {
    return round;
}

void Game::setRound(int _round) {
    Game::round = _round;
}

//private
void Game::clearClientsPoints() {
    round = 0;
    for (const auto &kv : Server::getUsersMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        kv.second->setScore(0);
    }
}

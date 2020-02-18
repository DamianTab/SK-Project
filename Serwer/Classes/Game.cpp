//
// Created by dejmian on 23.01.2020.
//

#include <Utils/utils.h>
#include <thread>
#include <cstring>
#include "Game.h"
#include "Server.h"
#include <thread>
#include <mutex>
#include <csignal>


int Game::round;
std::vector<Client *> Game::clientsRankingByTime;
Game *Game::gameInstance;

char startMessage[] = "The new game has started !!!";
char letterMessage[] = "New letter is : ";
char roundMessage[] = "Starting round: ";
char endMessage[] = "The game has ended. Not enough players to to continue :( ";

Game::Game() {
    printf("++++ Starting new game ... \n");
    gameInstance = this;
    clearClientsPoints(true);
//    New thread on this instance
    gameThread = std::thread(&Game::run, this);
    gameThread.detach();
}

Game::~Game() {
    clearClientsPoints(true);
    gameInstance = nullptr;
    printf("++++ The game has ended ... \n");
}

void Game::run() {
    srand(time(NULL));
    char receiveBuffer[BUFFER_SIZE];
    std::string tempString;
    incrementRound();

//    Sending greetings
    Server::sendToAllClients(startMessage);
    mutexClientsMap.lock();
    while ((int) Server::getClientsMap().size() >= minPlayersNumber) {
        mutexClientsMap.unlock();
        drawLetter();
        printf("++++ Drawn letter: %c ... \n", letter);

//        Sending letter message
        tempString = std::string(letterMessage);
        tempString += letter;
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);

//        Sending round number (to user view)
        tempString = std::string(roundMessage) + std::to_string(getRound());
        strcpy(receiveBuffer, tempString.c_str());
        Server::sendToAllClients(receiveBuffer);
        printf("++++ Start of round: %d ... \n", getRound());


        sleep(roundTime);
        printf("++++ End of round: %d ... \n", getRound());
//        Ignoring new messages from Clients
        incrementRound();

//        Removing inactive clients
        removeInactiveClients();

//        Calculating results
        calculateResults();

//        Sending result
        sendResults();

//        Clean up for next round
        clearClientsPoints();

//        Mutex for while condition
        mutexClientsMap.lock();
    }
//  Release mutex after while condition
    mutexClientsMap.unlock();

//    Sending farewell mesage
    Server::sendToAllClients(endMessage);
    printf("++++ Not enough players to continue !\n");
    delete this;
}

void Game::pushClientToTimeRankingWhenPossible(Client *client) {
    if (clientsRankingByTime.size() < 10) {
        clientsRankingByTime.push_back(client);
    }
}

// Getters and Setters
int Game::getRound() {
//    std::scoped_lock lock{mutexRound};
    return round;
}

void Game::incrementRound() {
    std::scoped_lock lock{mutexRound};
    round++;
}

void Game::setRound(int _round) {
    std::scoped_lock lock{mutexRound};
    Game::round = _round;
}

//private
void Game::clearClientsPoints(bool shouldClearTotalPointsAndRound) {
    std::scoped_lock lock{mutexClientsMap};
    clientsRankingByTime.clear();
    for (const auto &kv : Server::getClientsMap()) {
        kv.second->lastAnswers.clear();
        kv.second->lastScore.clear();
        if (shouldClearTotalPointsAndRound) {
            kv.second->setScore(0);
        }
    }
    if (shouldClearTotalPointsAndRound) {
        setRound(0);
    }
}

void Game::drawLetter() {
    letter = rand() % 26 + 97;
}


void Game::removeInactiveClients() {
    std::scoped_lock lock{mutexClientsMap};
    for (const auto &kv : Server::getClientsMap()) {
        if (kv.second->inactiveRoundsNumber >= ROUNDS_NUMBER_TO_REMOVE_INACTIVE_CLIENT) {
            printf("++++ Removing inactive player with login: '%s' with rounds inactive: %d\n", kv.first.c_str(),
                   kv.second->inactiveRoundsNumber);
            delete kv.second;
        } else if (kv.second->lastAnswers.size() == 0) {
            kv.second->inactiveRoundsNumber++;
        }
    }
}

void Game::calculateResults() {
    std::scoped_lock lock{mutexClientsMap};
    float coefficient = 1.0;
    for (auto it = clientsRankingByTime.begin(); it != clientsRankingByTime.end(); ++it) {
        Client *client = *it;
//            If player has been already disconnected then next client from ranking
        if (!Server::isInsideClientMap(client->getLogin())) {
            continue;
        }
//            This client already has default values (0);
        client->lastScore.clear();
        for (int i = 0; i < (int) client->lastAnswers.size(); ++i) {
            bool isAnswerRepeated = false;
//                If answer is correct
            if (client->lastAnswers[i][0] == letter) {
                for (const auto &kv : Server::getClientsMap()) {
//                        Have to be other client
                    if (kv.second == client) continue;
//                        If other client has the same answer
                   if( (int) kv.second->lastAnswers.size() >= i + 1 ){
                        if (kv.second->lastAnswers[i] == client->lastAnswers[i]) {
                            isAnswerRepeated = true;
                            break;
                        }
                    }
                }

                if (isAnswerRepeated) {
                    client->lastScore.push_back(CORRECT_REPEATED_ANSWER_POINTS * coefficient);
                } else {
                    client->lastScore.push_back(CORRECT_ANSWER_POINTS * coefficient);
                }
//                    If answer is incorrect
            } else {
                client->lastScore.push_back(0);
            }
        }
        coefficient -= 0.1;
        client->recalculateTotalScore();

    }
}

void Game::sendResults() {
    std::scoped_lock lock{mutexClientsMap};
    for (const auto &kv : Server::getClientsMap()) {
//        kv.second->sendAnswersAndPoints();
//        printf("--------------------- Player: '%s' Total Points: %f\n", kv.second->getLogin().c_str(),
//               kv.second->getScore());
        for(const auto &source : Server::getClientsMap()){
            std::string answers = source.second->getFormattedAnswers();
            kv.second->sendMessage(answers);
        }
    }
}

// Getters and Setters
char Game::getLetter() const {
    return letter;
}

void Game::setLetter(char letter) {
    Game::letter = letter;
}

std::vector<Client *> &Game::getClientsRankingByTime() {
    return clientsRankingByTime;
}

void Game::setClientsRankingByTime(std::vector<Client *> &clientsRankingByTime) {
    Game::clientsRankingByTime = clientsRankingByTime;
}

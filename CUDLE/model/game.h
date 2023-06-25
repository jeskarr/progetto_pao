#ifndef GAME_H
#define GAME_H

#include "game_set.h"


class game
{
private:
    unsigned int n_rounds;
    unsigned int n_attempts;
    game_set selected_set;
    QVector<bool>* won_rounds;      //each elem rapresent a round, if the elem == true it's a winning round

public:
    game(unsigned int rounds = 1, unsigned int attempts = 1, game_set elements = create_sample());

    game_set* getSet();
    unsigned int getRounds() const;
    unsigned int getAttempts() const;
    QVector<bool>* getWonRounds() const;

    void win(int r);
};

#endif // GAME_H

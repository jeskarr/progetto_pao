#include "game.h"

game::game(unsigned int rounds, unsigned int attempts, game_set elements) : n_rounds(rounds),
    n_attempts(attempts), selected_set(elements), won_rounds(new QVector<bool>(n_rounds, false)) { }


game_set* game::getSet()
{
    return &selected_set;
}


unsigned int game::getRounds() const
{
    return n_rounds;
}


unsigned int game::getAttempts() const
{
    return n_attempts;
}


QVector<bool>* game::getWonRounds() const
{
    return won_rounds;
}


//PRE: r is the index of a winning round
//POST: update the element r of won_rounds to true
void game::win(int r){
    (*won_rounds)[r] = true;
}

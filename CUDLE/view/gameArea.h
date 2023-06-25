#ifndef GAMEAREA_H
#define GAMEAREA_H

#include <QWidget>
#include <QComboBox>
#include <QTableWidget>
#include "../model/game_set.h"


//Qwidget which display the game itself (where to make the guess and the guesses already made)
class gameArea : public QWidget
{
Q_OBJECT

private:
    game_set* selected_set;
    base_elem* toGuess;             //element selected to be guessed
    unsigned int leftAttempts;

    QComboBox* input;
    QTableWidget* guesses;

public:
    gameArea(game_set* set, unsigned int attempts, QWidget *parent = nullptr);

    void setUpGUI();
    void guess();

    const base_elem* getGuess() const;
    QTableWidget* getGuessesTable() const;

public slots:
    void checkValidGuess();

signals:
    void wonRound(bool, QString = "");      //if ==true the round is won, ==false otherwise and QString contains the correct answer
    void selectedElem(base_elem*);
};

#endif // GAMEAREA_H

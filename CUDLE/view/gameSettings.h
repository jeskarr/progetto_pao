#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "setSelectionWindow.h"
#include <QSpinBox>
#include "../model/game.h"


//QDialog used to select the game features (set, rounds, attempts)
class gameSettings : public setSelectionWindow
{
Q_OBJECT

private:
    QSpinBox* rounds;
    QSpinBox* attempts;

    game_set selected_set;

public:
    gameSettings();

    void updateGUI();

public slots:
    void create_game();
    void setChanged(QString new_set);

signals:
    void gameReady(game*);
};

#endif // GAMESETTINGS_H

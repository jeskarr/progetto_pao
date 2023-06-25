#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QVector>
#include "gameArea.h"
#include "../model/game.h"
#include "endGame.h"
#include <QVBoxLayout>
#include <QKeyEvent>


//widget that display the game itself (gameArea) and some other relevant infos (e.g. current round)
class gameWindow : public QWidget
{
Q_OBJECT

private:
    game* selected_game;
    unsigned int currentRound;

    QLabel* intro;
    QLabel* round_label;
    QStackedWidget* stackedWidget;          //there is a gameArea widget for each round
    QVector<gameArea*> roundsWidgets;       //stores the pointer to the gameAera widget of the stackedWidget
    endGame* finalPage;

public:
    gameWindow(game* g, QWidget *parent = nullptr);
    void setUpGUI();
    void setUpRounds();

public slots:
    void nextRoundMsg(bool win, QString answer);
    void endedGame();
    void roundRecapSlot(unsigned int round);
    void keyPressEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void backHome();
};

#endif // GAMEWINDOW_H

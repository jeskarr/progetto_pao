#ifndef ENDGAME_H
#define ENDGAME_H

#include <QWidget>
#include <QHBoxLayout>
#include <QScrollArea>


//Widget which displays the result of the game
class endGame : public QWidget
{
Q_OBJECT

private:
    QVector<bool>* results;     //each element represent a round, if ==true it's a winning
    unsigned int tot_win;

    QHBoxLayout* checkPerRound;     //buttons used to check the performance of each individual round
    QScrollArea* checkPerRoundScroll;

public:
    endGame(QVector<bool> wonRounds, QWidget* parent = nullptr);

    void setUpGUI(bool win);
    bool checkWin();

signals:
    void goHome();
    void getRoundRecap(unsigned int);
};

#endif // ENDGAME_H

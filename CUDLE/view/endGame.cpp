#include "endGame.h"
#include <QPushButton>
#include <QLabel>

endGame::endGame(QVector<bool> wonRounds, QWidget* parent) : QWidget(parent),
    results(new QVector<bool>(wonRounds)), tot_win(0)
{
    checkPerRound = new QHBoxLayout();

    checkPerRoundScroll = new QScrollArea();
    QWidget* support = new QWidget();
    support->setLayout(checkPerRound);
    checkPerRoundScroll->setMaximumWidth(750);
    checkPerRoundScroll->setWidget(support);
    checkPerRoundScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    checkPerRoundScroll->setWidgetResizable(true);

    setUpGUI(checkWin());
}


bool endGame::checkWin()
{
    for(unsigned int i = 0; i< results->size(); i++){
        QPushButton* singleRound = new QPushButton();
        singleRound->setText("Round " + QString::number(i+1));
        connect(singleRound, &QPushButton::clicked, this, [this, i]{emit getRoundRecap(i);});
        checkPerRound->addWidget(singleRound);

        if ((*results)[i] == true){
            tot_win++;
            singleRound->setStyleSheet("QPushButton{border: 2px solid green;}");
        }
        else{
            singleRound->setStyleSheet("QPushButton{border: 2px solid red;}");
        }
    }

    return (tot_win > results->size() / 2);
}


//PRE: if win == true sets up victory gui, otherwise defeat
void endGame::setUpGUI(bool win)
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QLabel* points = new QLabel();
    points->setAlignment(Qt::AlignCenter);
    points->setText("You have won " + QString::number(tot_win) + "/" + QString::number(results->size()) + " rounds.");
    points->setStyleSheet("font-weight: bold; font-size: 18px;");
    layout->addWidget(points);

    QLabel* artwork = new QLabel();
    artwork->setAlignment(Qt::AlignCenter);
    if(win){
        QPixmap pic(":/assets/victoryImg.png");
        artwork->setPixmap(pic.scaledToWidth(300));
    }
    else{
        QPixmap pic(":/assets/defeatImg.png");
        artwork->setPixmap(pic.scaledToWidth(300));
    }
    layout->addWidget(artwork);

    QLabel* performance = new QLabel("Click below to see the performance of each round:");
    performance->setAlignment(Qt::AlignHCenter);
    layout->addWidget(performance);
    layout->addWidget(checkPerRoundScroll);

    QPushButton* home = new QPushButton();
    home->setText("Go home");
    home->setStyleSheet("QPushButton{border: 2px outset #969afa; background-color: #E3E4FC;}");
    connect(home, SIGNAL(clicked()), this, SIGNAL(goHome()));
    layout->addWidget(home);

    this->setLayout(layout);
}

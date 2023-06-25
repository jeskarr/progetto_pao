#include "gameWindow.h"
#include "gameArea.h"
#include <QCompleter>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QAbstractButton>

gameWindow::gameWindow(game* g, QWidget *parent) : QWidget(parent), selected_game(g), currentRound(1)
{
    setWindowTitle("CUDLE GAME");
    setUpGUI();
}


void gameWindow::setUpRounds()
{
    for(unsigned int i=0; i < selected_game->getRounds(); i++){
        gameArea* round = new gameArea(selected_game->getSet(), selected_game->getAttempts());
        roundsWidgets.push_back(round);
        stackedWidget->addWidget(round);
        selected_game->getSet()->remove_elem(*(roundsWidgets[i]->getGuess()));
        connect(roundsWidgets[i], SIGNAL(wonRound(bool,QString)), this, SLOT(nextRoundMsg(bool,QString)));
    }
}


void gameWindow::setUpGUI()
{
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);

    QPixmap image(":/assets/logo.svg");
    QLabel* artwork = new QLabel();
    artwork->setPixmap(image.scaledToWidth(500));
    artwork->setAlignment(Qt::AlignCenter);
    layout->addWidget(artwork);

    intro = new QLabel();
    intro->setText("Guess the " + selected_game->getSet()->getType() + " and have fun!");
    intro->setAlignment(Qt::AlignCenter);
    layout->addWidget(intro);

    round_label = new QLabel();
    round_label->setText("ROUND " + QString::number(currentRound) + "/" + QString::number(selected_game->getRounds()));
    round_label->setStyleSheet("font-weight: bold;");
    round_label->setAlignment(Qt::AlignCenter);
    layout->addWidget(round_label);

    stackedWidget = new QStackedWidget();
    setUpRounds();
    stackedWidget->setCurrentIndex(0);

    layout->addWidget(stackedWidget);

    this->setLayout(layout);
}


void gameWindow::nextRoundMsg(bool win, QString answer)
{
    QMessageBox msgBox;
    if(win==true){
        selected_game->win(stackedWidget->currentIndex());
        msgBox.setText("VICTORY!");
        msgBox.setInformativeText("Congratulation, you have won the round! \nKeep it up!");
        QPixmap pic(":/assets/victoryIcon.svg");
        msgBox.setIconPixmap(pic.scaledToHeight(50));
    }
    else{
        msgBox.setText("DEFEAT! \nThe answer was " + answer);
        msgBox.setInformativeText("Unfortunately, you have finished the attempts for this round. \nTry again with the next one.");
        QPixmap pic(":/assets/defeatIcon.svg");
        msgBox.setIconPixmap(pic.scaledToHeight(50));
    }
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    QAbstractButton* buttonY = msgBox.button(QMessageBox::Yes);
    buttonY->setText("Next");
    QAbstractButton* buttonN = msgBox.button(QMessageBox::No);
    buttonN->setText("Surrender");
    msgBox.exec();

    if (msgBox.clickedButton() == buttonY){
        currentRound++;
        if(currentRound > selected_game->getRounds()){        //end of the game
            intro->hide();
            round_label->setText("END OF THE GAME");
            round_label->setStyleSheet("QLabel{font-family: papyrus; color: #BFA468; font-weight: italic bold;}");
            finalPage = new endGame(*(selected_game->getWonRounds()));
            connect(finalPage, SIGNAL(goHome()), this, SLOT(endedGame()));
            connect(finalPage, SIGNAL(getRoundRecap(uint)), this, SLOT(roundRecapSlot(uint)));
            stackedWidget->addWidget(finalPage);
        }
        else{
            round_label->setText("ROUND " + QString::number(currentRound) + "/" + QString::number(selected_game->getRounds()));
        }
        stackedWidget->setCurrentIndex(stackedWidget->currentIndex()+1);
    }
    else if (msgBox.clickedButton() == buttonN){    //end of the game
        intro->hide();
        round_label->setText("END OF THE GAME");
        round_label->setStyleSheet("QLabel{font-family: papyrus; color: #BFA468; font-weight: italic bold;}");
        finalPage = new endGame(selected_game->getWonRounds()->mid(0, currentRound));
        connect(finalPage, SIGNAL(goHome()), this, SLOT(endedGame()));
        connect(finalPage, SIGNAL(getRoundRecap(uint)), this, SLOT(roundRecapSlot(uint)));
        stackedWidget->addWidget(finalPage);
        stackedWidget->setCurrentWidget(finalPage);
    }
}


void gameWindow::endedGame()
{
    this->close();
}


void gameWindow::roundRecapSlot(unsigned int round)
{
    QWidget* roundRec = new QWidget();
    QVBoxLayout* roundLayout = new QVBoxLayout();
    QLabel* intro = new QLabel();
    intro->setText("The answer was:");
    intro->setAlignment(Qt::AlignCenter);
    roundLayout->addWidget(intro);

    QLabel* answer = new QLabel();
    answer->setText(roundsWidgets[round]->getGuess()->getSolution());
    answer->setAlignment(Qt::AlignCenter);
    answer->setStyleSheet("QLabel{font-family: papyrus; color: #BFA468; font-weight: bold; font-size: 25px;}");
    roundLayout->addWidget(answer);

    roundLayout->addWidget(roundsWidgets[round]->getGuessesTable());

    roundRec->setLayout(roundLayout);
    roundRec->setWindowFlags(Qt::Window);
    roundRec->show();
}


void gameWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
        if(this->isFullScreen()) {
            this->showNormal();
        }
    }
}


void gameWindow::closeEvent(QCloseEvent* event)
{
    QMessageBox::StandardButton answer = QMessageBox::warning(this, "WARNING!", "Do you really want to close the window? \nAll results will be lost.",
                                                              QMessageBox::Yes | QMessageBox::No);
        if(answer == QMessageBox::Yes){
            event->accept();
            emit backHome();
        }
        else
            event->ignore();
}

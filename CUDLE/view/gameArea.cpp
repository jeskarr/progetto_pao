#include "gameArea.h"
#include "visitorInfo.h"
#include <QBoxLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QCompleter>
#include <QScrollArea>
#include <QScrollBar>
#include <QTransform>
#include <QPushButton>
#include <QFileInfo>
#include <QLabel>

gameArea::gameArea(game_set* set, unsigned int attempts, QWidget *parent) : QWidget(parent),
        selected_set(new game_set(*set)), toGuess(selected_set->extract_random()), leftAttempts(attempts)
{
    setUpGUI();
}


void gameArea::setUpGUI()
{
    QVBoxLayout* layout = new QVBoxLayout();

    visitorInfo visitor;
    toGuess->accept(visitor);
        layout->addWidget(visitor.getWidget());

    QHBoxLayout* input_layout = new QHBoxLayout();
    input_layout->setSpacing(0);

    input = new QComboBox();
    for(game_set::iterator it = selected_set->begin(); it != selected_set->end(); it++){
        input->addItem(it->getSolution());
    }
    input->setEditable(true);
    input->setCurrentText(QString());
    input->setInsertPolicy(QComboBox::NoInsert);
    input->completer()->setCompletionMode(QCompleter::PopupCompletion);
    input->completer()->popup()->setStyleSheet("background-color: rgba(191, 164, 104, 1);");

    QPushButton* guessButton = new QPushButton();
    guessButton->setDefault(true);
    guessButton->setStyleSheet("QPushButton{height: 25; border: 2px outset rgb(191, 164, 104);}");
    guessButton->setFocusPolicy(Qt::TabFocus);
    guessButton->setMaximumWidth(150);
    QPixmap pixmap(":/assets/enter.svg");
    QIcon buttonIcon(pixmap);
    guessButton->setIcon(buttonIcon);
    connect(guessButton, SIGNAL(clicked()), this, SLOT(checkValidGuess()));

    input_layout->addWidget(input);
    input_layout->addWidget(guessButton);
    layout->addLayout(input_layout);

    guesses = new QTableWidget();
    QVector<QString>* attributes = new QVector<QString>(*(selected_set->getAttributes()));
    attributes->insert(0, "YOUR GUESS");
    guesses->setColumnCount(attributes->size());
    guesses->setHorizontalHeaderLabels(*attributes);
    guesses->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(guesses);

    layout->setAlignment(Qt::AlignCenter);
    this->setLayout(layout);
}


//POST: update the qtablewidget storing all guesses and emit the signal wonRound
//if it's the correct answer or the attempts are over for that round
void gameArea::guess()
{
    guesses->insertRow(0);
    QTableWidgetItem* rowHeader = new QTableWidgetItem(QString::number(guesses->rowCount()));
    guesses->setVerticalHeaderItem(0, rowHeader);

    base_elem* selected_elem = selected_set->findElem(input->currentText());

    QTableWidgetItem* item = new QTableWidgetItem(selected_elem->getSolution());
    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
    guesses->setItem(0, 0, item);
    QVector<QString> correct_values = toGuess->getValues();
    QVector<QString> input_values = selected_elem->getValues();
    for(int i = 0; i < input_values.size(); i++){
        guesses->setItem(0, i+1, new QTableWidgetItem(input_values[i]));
        if (input_values[i] == correct_values[i]){
            guesses->item(0, i+1)->setBackground(Qt::green);
        }
        else if (input_values[i].contains(correct_values[i]) || correct_values[i].contains(input_values[i])) {
            guesses->item(0, i+1)->setBackground(Qt::yellow);
        }
        else{
            guesses->item(0, i+1)->setBackground(Qt::red);
        }
    }

    if (selected_elem == toGuess){
        emit wonRound(true);
    }
    else{
        input->removeItem(input->currentIndex());
        leftAttempts--;
        if(leftAttempts==0){
            emit wonRound(false, toGuess->getSolution());
        }
    }
    input->setCurrentText("");
}


const base_elem* gameArea::getGuess() const
{
    return toGuess;
}


QTableWidget* gameArea::getGuessesTable() const
{
    return guesses;
}


//POST: display a warning message if the input is not in the set or has already been guessed or
//already selected
void gameArea::checkValidGuess()
{
    if(input->findText(input->currentText()) == -1){        //invalid guess, display message of error
        QMessageBox::warning(this, "Element not valid!", "The element you have inserted isn't inside the set or has already been used. "
                                                         "\nTry checking the spelling or guess again.");
        input->setCurrentText("");
    }
    else{
        guess();
    }
}

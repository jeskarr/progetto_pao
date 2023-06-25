#include "gameSettings.h"
#include <QFormLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>

gameSettings::gameSettings() : selected_set(readSet("sample_set"))
{
    setWindowTitle("Game Settings");
    updateGUI();
}


void gameSettings::updateGUI()
{
    QLabel* logo = new QLabel();
    QPixmap logoPic(":/assets/logo.svg");
    logo->setPixmap(logoPic.scaledToHeight(100));
    logo->setAlignment(Qt::AlignHCenter);
    main_layout->insertWidget(1, logo);

    rounds = new QSpinBox;
    rounds->setMinimum(1);
    rounds->setMaximum(selected_set.size());             //the max of rounds is the dimension of the selected set (by default sample_set)

    attempts = new QSpinBox;
    attempts->setMinimum(1);
    attempts->setMaximum(selected_set.size());                    //the max of rounds is the dimension of the selected set (by default sample_set)

    QFormLayout* settings = new QFormLayout();
    settings->addRow("Number of rounds (max set dim.): ", rounds);
    settings->addRow("Number of attempts per round (max set dim.): ", attempts);
    main_layout->insertLayout(2, settings);

    buttons->button(QDialogButtonBox::Ok)->setText("Start playing");
    buttons->button(QDialogButtonBox::Ok)->disconnect();
    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(create_game()));
    connect(set_selection, SIGNAL(currentTextChanged(QString)), this, SLOT(setChanged(QString)));
}


void gameSettings::create_game()
{
    game* g = new game(rounds->value(), attempts->value(), selected_set);
    emit gameReady(g);
    this->close();
}


void gameSettings::setChanged(QString new_set)
{
    selected_set = readSet(new_set);
    rounds->setMaximum(selected_set.size());
    attempts->setMaximum(selected_set.size());
}

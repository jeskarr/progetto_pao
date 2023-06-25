#include "mainwindow.h"
#include "QVBoxLayout"
#include "QHBoxLayout"
#include "QPushButton"
#include "QLabel"
#include "QPixmap"
#include "setSelectionWindow.h"
#include "setEditWindow.h"
#include "setCreationWindow.h"
#include "gameSettings.h"
#include "gameWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    QWidget* window = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    QLabel* intro = new QLabel("WELCOME TO");
    intro->setStyleSheet("QLabel{font-family: papyrus; font-size: 30px; color: #BFA468; font-weight: bold;}");
    intro->setAlignment(Qt::AlignCenter);
    layout->addWidget(intro);

    QPixmap image(":/assets/logo.svg");
    QLabel* artwork = new QLabel();
    artwork->setPixmap(image.scaledToWidth(750));
    artwork->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->addWidget(artwork);

    QLabel* rules = new QLabel("<html><p>Create your own custom set or just try to guess a number between 1 and 50 with our sample_set.</p>"
                               "<p><b><i>PLAYING IS EASY!</i></b></p>"
                               "<p>Simply type in the name of an element of the set and it will reveal its attributes. "
                               "The color of the tiles will change to show how close your guess was to the element to find:"
                               "<ul><li><span style='color: green'>Green</span> indicates the property is an exact match.</li>"
                               "<li><span style='color: yellow'>Yellow</span> indicates partial match.</li>"
                               "<li><span style='color: red'>Red</span> indicates there is no overlap between your guess and the property. </li></ul></p></html");
    rules->setWordWrap(true);
    rules->setAlignment(Qt::AlignCenter);
    layout->addWidget(rules);

    QHBoxLayout* commands = new QHBoxLayout();
    QPushButton* new_set = new QPushButton("Create Set");
    commands->addWidget(new_set);
    connect(new_set, SIGNAL(clicked()), this, SLOT(createSet()));
    QPushButton* mod_set = new QPushButton("Modify Set");
    commands->addWidget(mod_set);
    connect(mod_set, SIGNAL(clicked()), this, SLOT(selectSet()));
    QPushButton* start = new QPushButton("Play");
    start->setDefault(true);
    commands->addWidget(start);
    connect(start, SIGNAL(clicked()), this, SLOT(selectSettings()));
    layout->addLayout(commands);

    window->setLayout(layout);
    setCentralWidget(window);
}


void MainWindow::createSet()
{
    setCreationWindow w;
    w.setMaximumSize(this->size().shrunkBy(QMargins(200,100,200,100)));
    w.exec();
}


void MainWindow::selectSet()
{
    setSelectionWindow w;
    connect(&w, SIGNAL(selectedSet(QString)), this, SLOT(editSet(QString)));
    w.exec();
}


void MainWindow::selectSettings()
{
    gameSettings w;
    connect(&w, SIGNAL(gameReady(game*)), this, SLOT(play(game*)));
    w.exec();
}


void MainWindow::editSet(QString set_name)
{
    setEditWindow w(set_name);
    w.setMaximumSize(this->size().shrunkBy(QMargins(200,100,200,100)));
    w.exec();
}


void MainWindow::play(game* g)
{
    hide();
    gameWindow* w = new gameWindow(g);
    connect(w, &gameWindow::backHome, this, [this]{this->show();});
    w->showFullScreen();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape && !event->isAutoRepeat()) {
        if(this->isFullScreen()) {
            this->showNormal();
        }
    }
}

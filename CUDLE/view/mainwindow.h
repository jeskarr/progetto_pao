#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../model/game.h"


class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void createSet();
    void selectSet();
    void selectSettings();
    void editSet(QString set_name);
    void play(game* g);
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // MAINWINDOW_H

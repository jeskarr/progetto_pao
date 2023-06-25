#include "./view/mainwindow.h"
#include <QFile>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile( ":/style.qss" );
    styleFile.open(QFile::ReadOnly );
    QString style(styleFile.readAll());
    a.setStyleSheet(style);
    MainWindow w;
    w.showFullScreen();
    return a.exec();
}

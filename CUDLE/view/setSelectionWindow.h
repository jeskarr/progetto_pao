#ifndef SETSELECTIONWINDOW_H
#define SETSELECTIONWINDOW_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QComboBox>


//QDialog used to select a set (from the memory, i.e. .json files)
class setSelectionWindow : public QDialog
{
Q_OBJECT

protected:
    QComboBox* set_selection;
    QDialogButtonBox* buttons;
    QVBoxLayout* main_layout;

public:
    setSelectionWindow(QDialog* parent = 0);

    void setUpGUI();

signals:
    void selectedSet(QString);
};

#endif // SETSELECTIONWINDOW_H

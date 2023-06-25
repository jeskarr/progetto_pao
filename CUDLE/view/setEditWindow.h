#ifndef SETEDITWINDOW_H
#define SETEDITWINDOW_H

#include "model/game_set.h"
#include "setCreationWindow.h"


//QDialog used to edit/delete an existing set
class setEditWindow : public setCreationWindow
{
Q_OBJECT

private:
    game_set selected_set;

public:
    setEditWindow(QString s);

    void loadSetInfo();

public slots:
    void delete_set();
    void confirm_changes();
};

#endif // SETEDITWINDOW_H

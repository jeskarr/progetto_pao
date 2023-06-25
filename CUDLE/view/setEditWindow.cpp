#include "setEditWindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>

setEditWindow::setEditWindow(QString s) : selected_set(readSet(s))
{
    setWindowTitle("Set Edit");
    loadSetInfo();
}


void setEditWindow::loadSetInfo()
{
    name->setText(selected_set.getSetName());
    name->setReadOnly(true);        //cannot change the name of the set
    type->setText(selected_set.getType());

    QVector<QString>* attributes = selected_set.getAttributes();
    n_attributes->setMinimum(attributes->size());
    n_attributes->setReadOnly(true);    //cannot change the number of attributes
    n_attributes->setButtonSymbols(QAbstractSpinBox::NoButtons);
    n_attributes->setStyleSheet("QSpinBox{background-color: rgb(222, 220, 247);}");
    for(int i = 0; i<attributes->size(); i++){
        QLineEdit* attribute_i = new QLineEdit();
        attribute_i->setText(attributes->at(i));
        attribute_i->setMinimumWidth(80);
        attr_names->addWidget(attribute_i);

        QLayoutItem* toDelete = attr_names->itemAt(0);      //delete the empty widgets created with setCreationWindow()
        delete toDelete->widget();
    }

    for(game_set::iterator it = selected_set.begin(); it != selected_set.end(); it++){
        save_elem(&(*it));
    }

    buttons->addButton(QDialogButtonBox::Discard);
    buttons->button(QDialogButtonBox::Ok)->setText("Modify set");
    buttons->button(QDialogButtonBox::Discard)->setText("Delete set");
    buttons->button(QDialogButtonBox::Ok)->disconnect();
    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(confirm_changes()));
    connect(buttons->button(QDialogButtonBox::Discard), SIGNAL(clicked()), this, SLOT(delete_set()));
}


void setEditWindow::delete_set()
{
    QMessageBox::StandardButton answer = QMessageBox::critical(this, "ATTENTION!", "Do you really want to delete the set? \nAll infos will be lost permanently.",
                                                              QMessageBox::Yes | QMessageBox::No);
    if(answer == QMessageBox::Yes){
        selected_set.deleteSet();
        close();    //close setEditWindow
    }
}


void setEditWindow::confirm_changes()
{
    QVector<QString>* attributes = getAttributesNames();
    if (!checkEmptyStrings(attributes) || name->text() == "" || type->text() == ""){
        QMessageBox::warning(this, "WARNING!", "Fill in all set's fields!");
    }
    else{
        game_set* input_set = new game_set(attributes, name->text(), type->text());
        for(auto it = elements.begin(); it != elements.end(); ++it){
            input_set->add_elem(**it);
        }
        input_set->saveSet();
        close();
    }
}

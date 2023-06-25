#ifndef SETCREATIONWINDOW_H
#define SETCREATIONWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QPushButton>
#include <QTableWidget>

class base_elem;


//QDialog used to create a new set
class setCreationWindow : public QDialog
{
Q_OBJECT

private:
    QPushButton* add_element;
    QTableWidget* elements_layout;

protected:
    QLineEdit* name;
    QLineEdit* type;
    QSpinBox* n_attributes;
    QHBoxLayout* attr_names;
    QDialogButtonBox* buttons;

    QVector<base_elem*> elements;

public:
    setCreationWindow(QDialog* parent = 0);

    void setUpGUI();

    QVector<QString>* getAttributesNames() const;

public slots:
    void setAttributes(int n_attr);
    void newElement();
    void save_elem(base_elem* e);
    void save_mod_elem(int index, QString name_mod, QVector<QString>* values_mod, QString extra_mod, int type);
    void modify_elem(int index);
    void remove_elem(int index);
    void checkValidSet();
};

bool checkEmptyStrings(QVector<QString>* strings);


#endif // SETCREATIONWINDOW_H

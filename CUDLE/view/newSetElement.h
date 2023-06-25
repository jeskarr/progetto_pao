#ifndef NEWSETELEMENT_H
#define NEWSETELEMENT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include "../model/base_elem.h"


//QDialog used to create a new element for a set
class newSetElement : public QDialog
{

Q_OBJECT

private:
    QVector<QString>* attributes;
    QLabel* extra_label;        //can be used if the element is not a "base" elem
    QPushButton* extraButton;               //can be used if the element is not a "base" elem

protected:
    QFormLayout* elemInfo;
    QLineEdit* name;
    QComboBox* type;
    QLineEdit* extraField;      //can be used if the element is not a "base" elem
    QHBoxLayout* values;
    QDialogButtonBox* buttons;

    void showRowElemInfo(int row);
    void hideRowElemInfo(int row);

public:
    newSetElement(QVector<QString>* attr_names, QDialog* parent = 0);

    void setUpGUI();

signals:
    void acceptElem(base_elem*);

public slots:
    void indexChanged(int i);
    void checkElem();
    void browse_image();
    void browse_quote();
};


#endif // NEWSETELEMENT_H

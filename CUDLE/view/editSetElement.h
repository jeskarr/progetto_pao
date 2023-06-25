#ifndef EDITSETELEMENT_H
#define EDITSETELEMENT_H

#include "newSetElement.h"


//QDialog used to modify an element of a set
class editSetElement : public newSetElement
{
Q_OBJECT

private:
    base_elem* selected_elem;
    int original_type;      //0 means the dynamic type of selected_elem (at the beginning) is base_elem, 1 is image_elem and 2 is quote_elem

public:
    editSetElement(QVector<QString>* attr_names, base_elem* e);

    void loadElemInfo();

public slots:
    void confirm_changes();

signals:
    void acceptChanges(QString input_name, QVector<QString>* input_values, QString extraField, int input_type);
};

#endif // EDITSETELEMENT_H

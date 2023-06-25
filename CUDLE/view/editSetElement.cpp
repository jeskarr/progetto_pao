#include "editSetElement.h"
#include "../model/quote_elem.h"
#include "../model/image_elem.h"
#include "visitorInfo.h"
#include <QPushButton>
#include <QMessageBox>

editSetElement::editSetElement(QVector<QString>* attr_names, base_elem* e)
    : newSetElement(attr_names), selected_elem(e), original_type(0)
{
    setWindowTitle("Element Edit");
    loadElemInfo();
}


void editSetElement::loadElemInfo()
{
    name->setText(selected_elem->getSolution());

    visitorInfo visitor;
    selected_elem->accept(visitor);
    if(dynamic_cast<image_elem*>(selected_elem))
        type->setCurrentIndex(1);
    else if(dynamic_cast<quote_elem*>(selected_elem))
        type->setCurrentIndex(2);
    extraField->setText(visitor.getAdditionalInfo());

    QVector<QString> values_elem = selected_elem->getValues();
    for(int i = 0; i<values_elem.size(); i++){
        QLineEdit* value_i = dynamic_cast<QLineEdit*>(values->itemAt(i)->widget());
        value_i->setText(values_elem[i]);
    }

    buttons->button(QDialogButtonBox::Ok)->setText("Modify");
    buttons->button(QDialogButtonBox::Ok)->disconnect();
    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(confirm_changes()));
}


void editSetElement::confirm_changes()
{
    QString input_name = name->text();
    int input_type = type->currentIndex();

    if(input_type==1 && extraField->text() == ""){
        QMessageBox::warning(this, "Missing image!", "Choose an image for this element or change input type!");
    }
    else if(input_type==2 && extraField->text() == ""){
        QMessageBox::warning(this, "Missing quote!", "Please enter a quote or change input type!");
    }
    else if(input_name.size() < 1){
        QMessageBox::warning(this, "WARNING!", "Add a valid name for the element!");
    }
    else {
        QVector<QString>* input_values = new QVector<QString>();
        unsigned int inputValuesCount = 0;
        for(int i=0; i < values->count(); i++){
            QLineEdit* value_i = dynamic_cast<QLineEdit*>(values->itemAt(i)->widget());
            if(value_i->text() == nullptr){
                QMessageBox::warning(this, "WARNING!", "Add a valid value for the attribute " + QString::number(i+1));
            }
            else{
                inputValuesCount++;
            }
            input_values->append(value_i->text());
        }
        if(input_values->size() == inputValuesCount){       //the element is valid
            emit acceptChanges(input_name, input_values, extraField->text(), (input_type==original_type)? -1 : input_type);
            close();
        }
    }
}


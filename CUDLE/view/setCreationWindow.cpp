#include "setCreationWindow.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QHeaderView>
#include <QScrollArea>
#include <QScrollBar>
#include "editSetElement.h"
#include "../model/game_set.h"
#include "../model/image_elem.h"
#include "../model/quote_elem.h"
#include "newSetElement.h"

setCreationWindow::setCreationWindow(QDialog* parent) : QDialog(parent)
{
    setUpGUI();
    setWindowTitle("New Set Creation");
    setModal(true);
}


void setCreationWindow::setUpGUI()
{
    QLabel* logo = new QLabel();
    QPixmap logoPic(":/assets/logo.svg");
    logo->setPixmap(logoPic.scaledToHeight(150));
    logo->setAlignment(Qt::AlignHCenter);

    name = new QLineEdit();
    type = new QLineEdit();
    type->setPlaceholderText("e.g. number, word, animal etc...");
    n_attributes = new QSpinBox();
    n_attributes->setAlignment(Qt::AlignCenter);
    n_attributes->setMinimum(1);
    connect(n_attributes, SIGNAL(valueChanged(int)), this, SLOT(setAttributes(int)));

    QFormLayout* setInfo = new QFormLayout;
    setInfo->addRow("Name: ", name);
    setInfo->addRow("Type of guess: ", type);
    setInfo->addRow("Number of attributes: ", n_attributes);

    QLabel* attributes_info = new QLabel();
    attributes_info->setText("Names of the attributes: ");
    QLineEdit* attribute1 = new QLineEdit();
    attribute1->setPlaceholderText("Attribute 1");
    attribute1->setMinimumWidth(80);
    attr_names = new QHBoxLayout();
    attr_names->addWidget(attribute1);

    QScrollArea* scrollAttr = new QScrollArea(this);
    QWidget* support = new QWidget();
    support->setLayout(attr_names);
    scrollAttr->setWidget(support);
    scrollAttr->setWidgetResizable(true);
    scrollAttr->setFrameShape(QAbstractScrollArea::NoFrame);

    elements_layout = new QTableWidget();
    elements_layout->setAlternatingRowColors(true);
    elements_layout->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    elements_layout->horizontalHeader()->hide();
    elements_layout->setFrameShape(QAbstractScrollArea::NoFrame);
    elements_layout->setShowGrid(false);
    elements_layout->setRowCount(0);
    elements_layout->setColumnCount(3);
    QLabel* elements_info = new QLabel();
    elements_info->setText("Elements: ");
    elements = QVector<base_elem*>();

    add_element = new QPushButton();
    add_element->setText("Add a new element to the set");
    connect(add_element, SIGNAL(clicked()), this, SLOT(newElement()));

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancel");
    buttons->button(QDialogButtonBox::Ok)->setText("Create");
    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(close()));
    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(checkValidSet()));

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addStretch();
    main_layout->addWidget(logo);
    main_layout->addLayout(setInfo);
    main_layout->addWidget(attributes_info);
    main_layout->addWidget(scrollAttr);
    main_layout->addWidget(elements_info);
    main_layout->addWidget(elements_layout);
    main_layout->addWidget(add_element);
    main_layout->addStretch();
    main_layout->addWidget(buttons);

    setLayout(main_layout);
}


void setCreationWindow::setAttributes(int n_attr)
{
    int current = attr_names->count();      //number of current attributes
    if (n_attr < current){
        //delete extra attributes
        for(int i = n_attr; i < current; i++) {
            QLayoutItem* toDelete = attr_names->itemAt(n_attr);
            delete toDelete->widget();

        }
    }
    else if(n_attr > current){
        //add extra attributes
        for(int i=current; i < n_attr; i++){
            QLineEdit* attribute_i = new QLineEdit();
            attribute_i->setPlaceholderText("Attribute " + QString::number(i+1));
            attribute_i->setMinimumWidth(80);
            attr_names->addWidget(attribute_i);
        }
    }
}


QVector<QString>* setCreationWindow::getAttributesNames() const
{
    QVector<QString>* attributes = new QVector<QString>();
    for(int i=0; i < attr_names->count(); i++){
        QLineEdit* attr_i = dynamic_cast<QLineEdit*>(attr_names->itemAt(i)->widget());
        attributes->append(attr_i->text());
    }
    return attributes;
}


void setCreationWindow::newElement()
{
    QVector<QString>* attributes = getAttributesNames();
    if(checkEmptyStrings(attributes) == true){
        newSetElement w(attributes, this);
        connect(&w, SIGNAL(acceptElem(base_elem*)), this, SLOT(save_elem(base_elem*)));
        w.setMaximumSize(this->size());
        w.exec();
    }
    else{
        QMessageBox::warning(this, "WARNING!", "Insert all attributes' names before creating an element!");
    }
}


//POST: return false if there is at least an empty string in the vector parameter
bool checkEmptyStrings(QVector<QString>* strings)
{
    bool isNotEmpty = true;
    for(auto it = strings->begin(); it != strings->end(); it++){
        if (it->isEmpty())
            isNotEmpty = false;
    }
    return isNotEmpty;
}


void setCreationWindow::checkValidSet()
{
    QVector<QString>* attributes = getAttributesNames();
    if (!checkEmptyStrings(attributes) || name->text() == "" || type->text() == ""){
        QMessageBox::warning(this, "WARNING!", "Fill in all the set's fields!");
    }
    else if(checkName(name->text())){
        QMessageBox::warning(this, "WARNING!", "There is already a set with this name! \nPlease change the name.");
    }
    else{
        game_set* input_set = new game_set(attributes, name->text().replace("/", "-"), type->text());
        for(auto it = elements.begin(); it != elements.end(); ++it){
            input_set->add_elem(**it);
        }
        input_set->saveSet();
        close();
    }
}


void setCreationWindow::save_elem(base_elem* e)
{
    bool takenName = false;
    //check if there is already an element with the same name in the set
    for(auto it = elements.begin(); it != elements.end(); it++){
        if((*it)->getSolution() == e->getSolution()){
            takenName = true;
            QMessageBox::critical(this, "ATTENTION!", "There is already an element with this name in the set."
                                                      "\nPlease create an element with a different name.");
        }
    }

    if(takenName == false){
        elements.append(e);
        int row_index = elements_layout->rowCount();
        elements_layout->insertRow(row_index);
        QPushButton* change_elem = new QPushButton("Modify");
        QPushButton* delete_elem = new QPushButton("Remove");
        connect(change_elem, &QPushButton::clicked, this, [=]{modify_elem(elements_layout->currentRow());});
        connect(delete_elem, &QPushButton::clicked, this, [=]{remove_elem(elements_layout->currentRow());});
        QTableWidgetItem* newItem = new QTableWidgetItem(e->getSolution());
        elements_layout->setItem(row_index, 0, newItem);
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
        QTableWidgetItem* newItemModifyButton = new QTableWidgetItem();
        elements_layout->setItem(row_index, 1, newItemModifyButton);
        elements_layout->setCellWidget(row_index, 1, change_elem);
        QTableWidgetItem* newItemRemoveButton = new QTableWidgetItem();
        elements_layout->setItem(row_index, 2, newItemRemoveButton);
        elements_layout->setCellWidget(row_index, 2, delete_elem);
    }
}


void setCreationWindow::remove_elem(int index)
{
    elements_layout->removeRow(index);
    elements.removeAt(index);
}


void setCreationWindow::modify_elem(int index)
{
    QVector<QString>* attributes = getAttributesNames();
    if(checkEmptyStrings(attributes) == true){
        editSetElement w(attributes, elements[index]);
        connect(&w, &editSetElement::acceptChanges, this,
                [=](QString name, QVector<QString>* values, QString extra, int type){save_mod_elem(index, name, values,extra, type);});
        w.setMaximumSize(this->size());
        w.exec();
    }
    else{
        QMessageBox::warning(this, "WARNING!", "Insert all attributes' names before modifying an element!");
    }
}


//POST: modify the pointed element in elements[index] according to the parameters.
//In particular, if type == 0 transform the element to a base_elem, if ==1 to a image_elem and if ==2 to quote_elem (if == -1 it has not been changed)
void setCreationWindow::save_mod_elem(int index, QString name_mod, QVector<QString>* values_mod, QString extra_mod, int type)
{
    bool takenName = false;
    //check if there is already an element with the same name in the set
    for(auto it = elements.begin(); it != elements.end(); it++){
        if((*it)->getSolution() == name_mod && elements[index] != *it){
            takenName = true;
            QMessageBox::critical(this, "ATTENTION!", "There is already an element with this name in the set."
                                                      "\nPlease keep the old name or change into a different one.");
        }
    }

    if(takenName == false){
        //to update the name of the element (if it has been changed)
        if(name_mod != elements[index]->getSolution()){
            QTableWidgetItem* newItem = new QTableWidgetItem(name_mod);
            elements_layout->setItem(index, 0, newItem);
        }

        //make the changes in the pointed element
        if(type != -1){     //the type has been changed
            base_elem* aux = elements[index];
            if(type==0)         //the node will point to a base_elem
                elements[index] = new base_elem(*aux);
            else if(type==1){        //the node will point to a image_elem
                elements[index] = new image_elem(new QVector<QString>(aux->getValues()), aux->getSolution(), "");
            }
            else if(type==2){        //the node will point to a quote_elem
                elements[index] = new quote_elem(new QVector<QString>(aux->getValues()), aux->getSolution(), "");
            }
            delete aux;
        }
        elements[index]->modify(name_mod, *(&values_mod), extra_mod);
    }
}

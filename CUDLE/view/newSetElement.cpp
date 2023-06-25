#include "newSetElement.h"
#include "../model/image_elem.h"
#include "../model/quote_elem.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>


newSetElement::newSetElement(QVector<QString>* attr_names, QDialog* parent) : QDialog(parent), attributes(attr_names)
{
    setUpGUI();
    setWindowTitle("New set's element creation");
    setModal(true);
}


void newSetElement::setUpGUI()
{
    QLabel* logo = new QLabel();
    QPixmap logoPic(":/assets/logo.svg");
    logo->setPixmap(logoPic.scaledToHeight(150));
    logo->setAlignment(Qt::AlignHCenter);

    name = new QLineEdit();
    type = new QComboBox();
    type->addItem("Standard element");
    type->addItem("Element with Image");
    type->addItem("Element with quote");
    type->setCurrentIndex(0);
    connect(type, SIGNAL(currentIndexChanged(int)), this, SLOT(indexChanged(int)));

    elemInfo = new QFormLayout;
    elemInfo->addRow("Name: ", name);
    elemInfo->addRow("Type: ", type);
    extra_label = new QLabel();
    extraField = new QLineEdit();
    extraField->setReadOnly(true);
    elemInfo->addRow(extra_label, extraField);       //the extra row is visible only when the index of the combobox is changed
    hideRowElemInfo(2);
    extraButton = new QPushButton();
    elemInfo->addRow(extraButton);
    hideRowElemInfo(3);

    //add tutti i values
    QLabel* values_info = new QLabel();
    values_info->setText("Values of the attributes: ");
    values = new QHBoxLayout();
    for(auto it = attributes->begin(); it != attributes->end(); ++it){
        QLineEdit* value = new QLineEdit();
        value->setPlaceholderText(*it);
        value->setMinimumWidth(80);
        values->addWidget(value);
    }

    QScrollArea* scroll = new QScrollArea(this);
    QWidget* support = new QWidget();
    support->setLayout(values);
    scroll->setWidget(support);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QAbstractScrollArea::NoFrame);

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancel");
    buttons->button(QDialogButtonBox::Ok)->setText("Create");
    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(close()));
    connect(buttons->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(checkElem()));

    //add the widgets to the main layout and set it
    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addStretch();
    main_layout->addWidget(logo);
    main_layout->addLayout(elemInfo);
    main_layout->addWidget(values_info);
    main_layout->addWidget(scroll);
    main_layout->addStretch();
    main_layout->addWidget(buttons);

    setLayout(main_layout);
}


void newSetElement::indexChanged(int i)
{
    extraButton->disconnect();
    extraField->clear();

    if(i==0){
        hideRowElemInfo(2);
        hideRowElemInfo(3);
        extra_label->setText("");
    }
    else{  
        showRowElemInfo(2);     //shows extraLabel-extraField
        showRowElemInfo(3);     //shows extraButton
        if (i==1){      //image elem   
            extra_label->setText("Image: ");
            extraField->setReadOnly(true);
            extraButton->setText("Select image");
            connect(extraButton, SIGNAL(clicked()), this, SLOT(browse_image()));
        }
        if (i==2){      //quote elem
            extra_label->setText("Quote: ");
            extraField->setReadOnly(false);
            extraButton->setText("Select quote from file");
            connect(extraButton, SIGNAL(clicked()), this, SLOT(browse_quote()));
        }
    }
}


//POST: check if the element inputted is valid: if it is signals it to the setCreation(/Edit)Window
//to add it to the set, otherwise displays a warning message
void newSetElement::checkElem()
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
            if(input_type==0){
                emit acceptElem(new base_elem(input_values, input_name));
            }
            else if(input_type==1){
                emit acceptElem(new image_elem(input_values, input_name, extraField->text()));
            }
            else if(input_type==2){
                emit acceptElem(new quote_elem(input_values, input_name, extraField->text()));
            }
            close();
        }
    }
}


//POST: consent to select an image (.png or .jpg only) from the pc
void newSetElement::browse_image()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose File", "/Desktop", "Images(*.png *.jpg)");
    extraField->setText(filename);
}


//POST: consent to select a quote (.txt only) from the pc
void newSetElement::browse_quote()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose File", "/Desktop", "Text(*.txt)");
    QFile file(filename);
    extraField->clear();
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        extraField->setText(file.readAll());
    file.close();
}


//POST: make visible the specified row in the QFormLayout elemInfo
void newSetElement::showRowElemInfo(int row)
{
    elemInfo->itemAt(row, QFormLayout::FieldRole)->widget()->show();
}


//POST: hide the specified row in the QFormLayout elemInfo
void newSetElement::hideRowElemInfo(int row)
{
    elemInfo->itemAt(row, QFormLayout::FieldRole)->widget()->hide();
}

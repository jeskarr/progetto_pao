#include "setSelectionWindow.h"
#include "../model/game_set.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

setSelectionWindow::setSelectionWindow(QDialog* parent) : QDialog(parent)
{
    setUpGUI();
    setWindowTitle("Set Selection");
    setModal(true);
}


void setSelectionWindow::setUpGUI()
{
    QLabel* info = new QLabel();
    info->setText("Select a set:");

    set_selection = new QComboBox();
    QVector<QString> sets_names = readSetsNames();
    for(auto it = sets_names.begin(); it != sets_names.end(); ++it){
        set_selection->addItem(*it);
    }
    set_selection->setCurrentText("sample_set");

    buttons = new QDialogButtonBox();
    buttons->addButton(QDialogButtonBox::Cancel);
    buttons->addButton(QDialogButtonBox::Ok);
    buttons->button(QDialogButtonBox::Cancel)->setText("Cancel");
    buttons->button(QDialogButtonBox::Ok)->setText("Edit");
    connect(buttons->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(close()));
    connect(buttons->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, [this]{close(); emit selectedSet(set_selection->currentText());});

    main_layout = new QVBoxLayout();
    main_layout->addStretch();
    main_layout->addWidget(info);
    main_layout->addWidget(set_selection);
    main_layout->addStretch();
    main_layout->addWidget(buttons);

    setLayout(main_layout);
}

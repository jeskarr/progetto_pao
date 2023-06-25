#include "visitorInfo.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QScrollArea>
#include <QPixmap>
#include "../model/quote_elem.h"
#include "../model/image_elem.h"

QWidget* visitorInfo::getWidget() const
{
    return widget;
}


QString visitorInfo::getAdditionalInfo() const
{
    return info;
}


void visitorInfo::visitBase(const base_elem& base_el)
{
    widget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* description = new QLabel("Type in a random element and then guess the correct one by looking at the values of the attributes.");
    description->setWordWrap(true);
    description->setAlignment(Qt::AlignCenter);
    description->setStyleSheet("font-style: italic;");
    layout->addWidget(description);
    QLabel* help = new QLabel("See Clue");
    help->setToolTip("The element starts with: " + QString(base_el.getSolution().front()));
    help->setStyleSheet("color: rgb(191, 164, 104);");
    help->setAlignment(Qt::AlignCenter);
    layout->addWidget(help);
    widget->setLayout(layout);
}


void visitorInfo::visitImage(const image_elem& image_el)
{
    widget = new QScrollArea();
    QLabel* artwork = new QLabel();
    QPixmap img(image_el.getImage());
    if(img.isNull())
        img.load(":/assets/imgPlaceholder.png");
    artwork->setPixmap(img.scaledToHeight(300));
    artwork->setAlignment(Qt::AlignCenter);
    (dynamic_cast<QScrollArea*>(widget))->setWidget(artwork);
    (dynamic_cast<QScrollArea*>(widget))->setWidgetResizable(true);

    info = image_el.getImage();
}


void visitorInfo::visitQuote(const quote_elem& quote_el)
{
    widget = new QWidget;
    QHBoxLayout* quoteBox = new QHBoxLayout();
    quoteBox->setAlignment(Qt::AlignCenter);
    quoteBox->setSpacing(0);

    QPixmap quoteImg(":/assets/quote1.svg");
    QLabel* quoteUp = new QLabel();
    quoteUp->setPixmap(quoteImg.scaledToHeight(25));
    quoteUp->setAlignment(Qt::AlignTop);
    quoteUp->setStyleSheet("QLabel{background-color: rgb(220,220,220);}");
    quoteBox->addWidget(quoteUp);

    QScrollArea* scrollQuote = new QScrollArea();
    scrollQuote->setMaximumHeight(200);
    scrollQuote->setStyleSheet("border: none; background-color: rgb(220,220,220);");
    QLabel* quote = new QLabel();
    quote->setText(quote_el.getQuote());
    quote->setWordWrap(true);
    quote->setMargin(5);
    quote->setAlignment(Qt::AlignCenter);
    scrollQuote->setWidget(quote);
    scrollQuote->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollQuote->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollQuote->setWidgetResizable(true);
    scrollQuote->setMinimumWidth(quote->minimumSizeHint().width());
    quoteBox->addWidget(scrollQuote);

    QPixmap quoteImg_mirrored(":/assets/quote2.svg");
    QLabel* quoteDown = new QLabel();
    quoteDown->setPixmap(quoteImg_mirrored.scaledToHeight(25));
    quoteDown->setAlignment(Qt::AlignBottom);
    quoteDown->setStyleSheet("QLabel{background-color: rgb(220,220,220);}");
    quoteBox->addWidget(quoteDown);

    widget->setLayout(quoteBox);

    info = quote_el.getQuote();
}

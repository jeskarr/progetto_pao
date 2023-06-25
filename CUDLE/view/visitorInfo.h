#ifndef VISITORINFO_H
#define VISITORINFO_H

#include "../model/visitorInterface.h"
#include <QWidget>
#include <QString>


class visitorInfo : public visitorInterface
{
private:
    QWidget* widget;        //additional widget displayed in-game
    QString info;        //additional info (image path or cit)

public:
    QWidget* getWidget() const;
    QString getAdditionalInfo() const;

    virtual void visitBase(const base_elem& base_el) override;
    virtual void visitImage(const image_elem& image_el) override;
    virtual void visitQuote(const quote_elem& quote_el) override;
};

#endif // VISITORINFO_H

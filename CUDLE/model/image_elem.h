#ifndef IMAGE_ELEM_H
#define IMAGE_ELEM_H

#include "base_elem.h"


class image_elem : public base_elem
{
private:
    QString image;

public:
    image_elem(QVector<QString>* values, QString sol, QString pic);

    QString getImage() const;

    virtual bool operator==(const base_elem& e) const override;
    virtual bool operator!=(const base_elem& e) const override;

    virtual void accept(visitorInterface& visitor) const override;
    virtual void modify(QString mod_sol, QVector<QString>* mod_values, QString extra) override;
};

#endif // IMAGE_ELEM_H

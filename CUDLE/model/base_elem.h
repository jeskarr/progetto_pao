#ifndef BASE_ELEM_H
#define BASE_ELEM_H

#include <QString>
#include <QVector>
#include "visitorInterface.h"


class base_elem
{
private:
    QVector<QString> attributes_values;
    QString solution;

public:
    base_elem() = default;
    base_elem(QVector<QString>* values, QString sol);
    virtual ~base_elem();

    QVector<QString> getValues() const;
    QString getSolution() const;

    virtual bool operator==(const base_elem& e) const;
    virtual bool operator!=(const base_elem& e) const;

    virtual void accept(visitorInterface& visitor) const;
    virtual void modify(QString mod_sol, QVector<QString>* mod_values, QString extra = "");
};

#endif // BASE_ELEM_H

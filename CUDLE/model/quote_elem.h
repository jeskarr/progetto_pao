#ifndef QUOTE_ELEM_H
#define QUOTE_ELEM_H

#include "base_elem.h"


class quote_elem : public base_elem
{
private:
    QString quote;

public:
    quote_elem(QVector<QString>* values, QString sol, QString cit);

    QString getQuote() const;

    virtual bool operator==(const base_elem& e) const override;
    virtual bool operator!=(const base_elem& e) const override;

    virtual void accept(visitorInterface& visitor) const override;
    virtual void modify(QString mod_sol, QVector<QString>* mod_values, QString extra) override;
};

#endif // QUOTE_ELEM_H

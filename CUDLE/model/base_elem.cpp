#include "base_elem.h"

base_elem::base_elem(QVector<QString>* values, QString sol)
    : attributes_values(*values), solution(sol) { }


base_elem::~base_elem() { }


QVector<QString> base_elem::getValues() const
{
    return attributes_values;
}


QString base_elem::getSolution() const
{
    return solution;
}


bool base_elem::operator==(const base_elem& e) const
{
    return (e.attributes_values == this->attributes_values) && (e.solution == this->solution);
}


bool base_elem::operator!=(const base_elem& e) const
{
    return (e.attributes_values != this->attributes_values) && (e.solution != this->solution);
}


void base_elem::accept(visitorInterface& visitor) const
{
    visitor.visitBase(*this);
}


void base_elem::modify(QString mod_sol, QVector<QString>* mod_values, QString extra)
{
    solution = mod_sol;
    attributes_values = *mod_values;
    extra = "";
}

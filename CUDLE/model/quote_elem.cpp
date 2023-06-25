#include "quote_elem.h"

quote_elem::quote_elem(QVector<QString>* values, QString sol, QString cit)
    : base_elem(values, sol), quote(cit) { }


QString quote_elem::getQuote() const
{
    return quote;
}


bool quote_elem::operator==(const base_elem& e) const
{
    try{
        return base_elem::operator==(e) && ((dynamic_cast<const quote_elem&>(e)).quote == this->quote);
    }
    catch(std::bad_cast&){   //e is not an quote_object...
        return false;       //...so it cannot be == to one of them (i.e. *this)
    }
}


bool quote_elem::operator!=(const base_elem& e) const
{
    try{
        return base_elem::operator!=(e) && ((dynamic_cast<const quote_elem&>(e)).quote != this->quote);
    }
    catch(std::bad_cast&){   //e is not an quote_object...
        return true;       //...so it is for sure != to one of them (i.e. *this)
    }
}


void quote_elem::accept(visitorInterface& visitor) const
{
    visitor.visitQuote(*this);
}


//PRE: if the parameters are empty it means there are no changes
void quote_elem::modify(QString mod_sol, QVector<QString>* mod_values, QString extra)
{
    base_elem::modify(mod_sol, *(&mod_values));
    if (extra != "")
        quote = extra;
}

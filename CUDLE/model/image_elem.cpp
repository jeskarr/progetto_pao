#include "image_elem.h"

image_elem::image_elem(QVector<QString>* values, QString sol, QString pic)
    : base_elem(values, sol), image(pic) { }


QString image_elem::getImage() const
{
    return image;
}


bool image_elem::operator==(const base_elem& e) const
{
    try{
        return base_elem::operator==(e) && ((dynamic_cast<const image_elem&>(e)).image == this->image);
    }
    catch(std::bad_cast&){   //e is not an image_object...
        return false;       //...so it cannot be == to one of them (i.e. *this)
    }
}


bool image_elem::operator!=(const base_elem& e) const
{
    try{
        return base_elem::operator!=(e) && ((dynamic_cast<const image_elem&>(e)).image != this->image);
    }
    catch(std::bad_cast&){   //e is not an image_object...
        return true;       //...so it is for sure != to one of them (i.e. *this)
    }
}


void image_elem::accept(visitorInterface& visitor) const
{
    visitor.visitImage(*this);
}


//PRE: if the parameters are empty it means there are no changes
void image_elem::modify(QString mod_sol, QVector<QString>* mod_values, QString extra)
{
    base_elem::modify(mod_sol, *(&mod_values));
    if (extra != "")
        image = extra;
}


#ifndef VISITORINTERFACE_H
#define VISITORINTERFACE_H

class image_elem;
class base_elem;
class quote_elem;


class visitorInterface
{
public:
    ~visitorInterface() = default;

    virtual void visitBase(const base_elem& base_el) = 0;
    virtual void visitImage(const image_elem& image_el) = 0;
    virtual void visitQuote(const quote_elem& quote_el) = 0;
};

#endif // VISITORINTERFACE_H

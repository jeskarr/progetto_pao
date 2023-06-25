#include "game_set.h"
#include "memoryManagement.h"
#include "image_elem.h"
#include "quote_elem.h"

game_set::node::node() : next(nullptr) {}
game_set::node::node(base_elem& e, node* p) : data(&e), next(p) { }


game_set::node* game_set::copy(node* p)
{
    if(!p)
        return nullptr;
    else
        return new node(*(p->data), copy(p->next));
}


void game_set::destroy(node* p)
{
    if(p){
        destroy(p->next);
        delete p;
    }
}


game_set::game_set(QVector<QString>* attr_names, QString name, QString type) :
    set_name(name), object_type(type), attributes(attr_names), firstElem(nullptr) { }


game_set::~game_set()
{
    destroy(firstElem);
}


game_set::game_set(const game_set& s) : set_name(s.set_name), object_type(s.object_type),
    attributes(s.attributes), firstElem(copy(s.firstElem)) { }


game_set& game_set::operator=(const game_set& s)
{
    if(this != &s){
        destroy(firstElem);
        firstElem = copy(s.firstElem);

        set_name = s.set_name;
        object_type = s.object_type;
        attributes = s.attributes;
    }
    return *this;
}


QString game_set::getSetName() const
{
    return set_name;
}


QString game_set::getType() const
{
    return object_type;
}


QVector<QString>* game_set::getAttributes() const
{
    return attributes;
}


bool game_set::empty() const
{
    return firstElem == nullptr;
}


unsigned int game_set::size() const
{
    unsigned int dim = 0;
    for(game_set::iterator it = this->begin(); it != this->end(); ++it)
        dim ++;
    return dim;
}


void game_set::add_elem(base_elem& e)
{
    firstElem = new node(e, firstElem);
}


void game_set::remove_elem(const base_elem& e)
{
    node* curr = firstElem;
    node* prec = nullptr;
    while(curr && !(*(curr->data) == e)){
        prec = curr;
        curr = curr->next;
    }
    if(curr){        //element e founded
        if(!prec)       //if e is FirstElem
            firstElem = curr->next;
        else
            prec->next = curr->next;
    delete curr;
    }
}


//PRE: game_set is not empty
//POST: return a random pointer to an element of the game_set,
//If the game_set is empty returns nullptr.
base_elem* game_set::extract_random() const
{
    srand(time(NULL));
    if (size() > 0){
        int random_index = rand() % size();      //size() is the tot number of elements in the game_set
        iterator it = this->begin();
        for( ; it!= this->end() && random_index != 0; ++it){
            random_index--;
        }
        return &(*it);
    }
    return nullptr;
}


//POST: returns a pointer to a base_elem with the solution value == parameter e
//If there isn't any, return nullptr.
base_elem* game_set::findElem(QString e) const
{
    for(iterator it = this->begin(); it != this->end(); ++it){
        if (it->getSolution() == e){
            return &(*it);
        }
    }
    return nullptr;
}


bool game_set::iterator::operator==(const iterator& i) const
{
    return punt == i.punt;
}


bool game_set::iterator::operator!=(const iterator& i) const
{
    return punt != i.punt;
}


game_set::iterator& game_set::iterator::operator++()
{
    if(punt)
        punt = punt->next;
    return *this;
}


game_set::iterator game_set::iterator::operator++(int)
{
    game_set::iterator aux = *this;
    if(punt)
        punt = punt->next;
    return aux;
}


base_elem* game_set::iterator::operator->() const
{
    return punt->data;
}


base_elem& game_set::iterator::operator*() const
{
    return *(punt->data);
}


game_set::iterator game_set::begin() const
{
    game_set::iterator aux;
    aux.punt = firstElem;
    return aux;
}


game_set::iterator game_set::end() const
{
    game_set::iterator aux;
    aux.punt = nullptr;
    return aux;
}


//POST: delete the file containing the details of the game_set object
void game_set::deleteSet() const
{
    QFile file(setsDetails + QString("/") + set_name + QString(".json"));
    file.remove();
}


//POST: save (or update) the details of the game_set object into a json file
void game_set::saveSet() const
{
    QJsonDocument doc;
    QJsonObject set_obj;

    set_obj.insert("type", object_type);

    QJsonArray attr_arr;
    for(auto it = attributes->begin(); it != attributes->end(); ++it){
        attr_arr.push_back(*it);
    }
    set_obj.insert("attributes", attr_arr);

    QJsonArray elements_arr;
    for(iterator it = this->begin(); it != this->end(); ++it){
        QJsonArray values_arr;
        QVector<QString> elem_values = it->getValues();
        for(auto values_it = elem_values.begin(); values_it != elem_values.end(); ++values_it){
            values_arr.push_back(*values_it);
        }
        QJsonObject single_element{{"solution", it->getSolution()}, {"values", values_arr}};

        image_elem* imgCast = dynamic_cast<image_elem*>(&(*it));
        quote_elem* citCast = dynamic_cast<quote_elem*>(&(*it));
        if (imgCast)
            single_element.insert("image", imgCast->getImage());
        else if (citCast)
            single_element.insert("quote", citCast->getQuote());
        elements_arr.push_back(single_element);
    }
    set_obj.insert("elements", elements_arr);

    doc.setObject(set_obj);
    write(setsDetails + QString("/") + set_name + QString(".json"), doc);
}


//POST: create a game_set object which elements are the numbers from 1 to 50
//and values are if the number is even or odd
game_set create_sample()
{
    QVector<QString>* attributes = new QVector<QString>();
    attributes->push_back("Even/Odd");
    game_set ret(attributes, "sample_set", "number");
    for(int i=1; i<=50; ++i){
        QVector<QString>* values = new QVector<QString>();
        values->push_back((i%2==0) ? "Even" : "Odd");
        base_elem* e = new base_elem(values, QString::number(i));
        ret.add_elem(*e);
    }

    if(! QFileInfo::exists(setsDetails + QString("/") + ret.getSetName() + QString(".json")))
        ret.saveSet();

    return ret;
}


//POST: returns a vector with all the sets name (from the stored .json files)
QVector<QString> readSetsNames()
{
    QDir directory(setsDetails);
    QStringList setsnames = directory.entryList(QStringList() << "*.json", QDir::Files | QDir::NoDot | QDir::NoDotDot);
    setsnames.replaceInStrings(".json", "");
    //always check that the sample_set exists, if it doesn't create (and save) it
    if(!setsnames.contains("sample_set")){
        create_sample();
        setsnames << "sample_set";
    }

    return setsnames;
}


//POST: return 1 if the name is already used for a game_set, 0 otherwise
bool checkName(QString name)
{
    return readSetsNames().contains(name);
}


//PRE: name is a real set name
//POST: from the details stored inside the .json file creates a game_set object
game_set readSet(QString name)
{
    QJsonDocument doc;
    try {
        doc = read(setsDetails + QString("/") + name + ".json");
    }
    catch (...) {
        //if it doesn't work, create the sample_set
        return create_sample();
    }
    QJsonObject set_obj = doc.object();

    QString type = set_obj["type"].toString();
    QVector<QString>* attributes = new QVector<QString>;
    QJsonArray attr_jsonArr = set_obj["attributes"].toArray();
    for(auto it = attr_jsonArr.begin(); it != attr_jsonArr.end(); ++it){
        attributes->push_back(it->toString());
    }

    game_set ret(attributes, name, type);

    QJsonArray elements_jsonArr = set_obj["elements"].toArray();
    for(auto it = elements_jsonArr.begin(); it != elements_jsonArr.end(); ++it){
        QJsonObject elem_obj = it->toObject();

        QString solution = elem_obj["solution"].toString();
        QVector<QString>* values = new QVector<QString>;
        QJsonArray values_jsonArr = elem_obj["values"].toArray();
        for(auto it = values_jsonArr.begin(); it != values_jsonArr.end(); ++it){
            values->push_back(it->toString());
        }
        if(elem_obj.contains("image")){
            image_elem* e = new image_elem(values, solution, elem_obj["image"].toString());
            ret.add_elem(*e);
        }
        else if(elem_obj.contains("quote")){
            quote_elem* e = new quote_elem(values, solution, elem_obj["quote"].toString());
            ret.add_elem(*e);
        }
        else{
            base_elem* e = new base_elem(values, solution);
            ret.add_elem(*e);
        }
    }
    return ret;
}

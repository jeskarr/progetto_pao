#ifndef GAME_SET_H
#define GAME_SET_H

#define setsDetails "../CUDLE/db"

#include <QVector>
#include <QString>
#include "base_elem.h"


class game_set      //contenitor of the elements of the set
{
private:
    class node      //contains a polymorphic pointer to the element + pointer to the next element in the container
    {
    public:
        node();
        node(base_elem& e, node* p);
        base_elem* data;
        node* next;
    };

    QString set_name;
    QString object_type;
    QVector<QString>* attributes;
    node* firstElem;        //pointer to the first element of the container

    static node* copy(node* p);
    static void destroy(node* p);

public:
    class iterator
    {
    friend class game_set;
    private:
        game_set::node* punt;

    public:
        bool operator==(const iterator& i) const;
        bool operator!=(const iterator& i) const;
        iterator& operator++();     //operator++ prefixed
        iterator operator++(int);   //operator++ postfixed
        base_elem* operator->() const;
        base_elem& operator*() const;
    };

    game_set(QVector<QString>* attr_names, QString name = "New Set", QString type = "word");
    ~game_set();    //profound destruction
    game_set(const game_set& s);    //profound copy constructor
    game_set& operator=(const game_set& s);     //profound assignment

    QString getSetName() const;
    QString getType() const;
    QVector<QString>* getAttributes() const;
    bool empty() const;
    unsigned int size() const;
    void add_elem(base_elem& e);
    void remove_elem(const base_elem& e);
    base_elem* extract_random() const;
    base_elem* findElem(QString e) const;

    iterator begin() const;
    iterator end() const;

    void deleteSet() const;
    void saveSet() const;
};

game_set create_sample();
QVector<QString> readSetsNames();
game_set readSet(QString name);
bool checkName(QString name);


#endif // GAME_SET_H

#ifndef HANDS_H
#define HANDS_H
#include "dominoshka.h"

class Hands
{
private:
    int count;
    Dominoshka* dominoes;

public:
    Hands();
    ~Hands();

    void add(Dominoshka dom);
    Dominoshka& at(int index);
    int length();
    void del(int index);
};

#endif // HANDS_H

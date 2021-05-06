#include "hands.h"
#include <cstring>

Hands::Hands()
{
    count = 0;
    dominoes = nullptr;
}

Hands::~Hands()
{
    delete [] dominoes;
}

void Hands::add(Dominoshka dom)
{
    Dominoshka* new_arr;
    try
    {
        new_arr = new Dominoshka[count + 1];
    }
    catch (...)
    {
        throw 777;
    }
    memcpy(new_arr, dominoes, count * sizeof(Dominoshka));
    new_arr[count] = dom;
    count++;
    delete[] dominoes;
    dominoes = new_arr;
}

Dominoshka& Hands::at(int index)
{
    return dominoes[index];
}

int Hands::length()
{
    return count;
}

void Hands::del(int index)
{
    Dominoshka* new_arr;
    try
    {
        new_arr = new Dominoshka[count - 1];
    }
    catch (...)
    {
        throw 777;
    }
    bool flag = false;
    for (int i = 0; i < count; i++)
        if (i != index)
            new_arr[i-flag] = dominoes[i];
        else
            flag = true;
    count--;
    delete[] dominoes;
    dominoes = new_arr;
}

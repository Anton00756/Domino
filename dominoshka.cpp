#include "dominoshka.h"

Dominoshka::Dominoshka()
{}

Dominoshka::Dominoshka(int l, int r)
{
    left = l;
    right = r;
    horizontal = (l!=r);
}

void Dominoshka::swap_value()
{
    int temp = left;
    left = right;
    right = temp;
}

int Dominoshka::left_value()
{
    return left;
}

int Dominoshka::right_value()
{
    return right;
}

bool Dominoshka::rotation_value()
{
    return horizontal;
}

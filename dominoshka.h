#ifndef DOMINOSHKA_H
#define DOMINOSHKA_H

class Dominoshka
{
private:
    int left, right;
    bool horizontal;

public:
    Dominoshka();
    Dominoshka(int l, int r);
    int left_value();
    int right_value();
    bool rotation_value();
    void swap_value();
};

#endif // DOMINOSHKA_H

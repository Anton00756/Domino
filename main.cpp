#include "domino.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Domino w;
    w.show();
    return a.exec();
}

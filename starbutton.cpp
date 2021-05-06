#include "starbutton.h"

#include <QPixmap>

StarButton::StarButton(char aspect, QWidget *parent) : QLabel(parent)
{
    side = aspect;
    if (aspect == 'l')
        this->setGeometry(0, 300, 256, 256);
    else
        this->setGeometry(744, 300, 256, 256);
    this->setPixmap(QPixmap(":/resources/pictures/star.png").scaled(this->size(), Qt::IgnoreAspectRatio));
    this->hide();
}

void StarButton::mousePressEvent(QMouseEvent *event)
{
    emit star_clicked(side);
    event->accept();
}

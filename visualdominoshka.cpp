#include "visualdominoshka.h"

#include <QPixmap>
#include <QPainter>

VisualDominoshka::VisualDominoshka(int i, Dominoshka& dom, QWidget *parent) : QLabel(parent)
{
    index = i;
    this->setGeometry(300+50*i, 690, 45, 95);
    QPixmap visual_domino(":/resources/pictures/BaseDom.png");
    QPainter p(&visual_domino);
    if (dom.left_value() != 0)
        p.drawPixmap(0,0,QPixmap(QString(":/resources/pictures/%1.png").arg(dom.left_value())));
    if (dom.right_value() != 0)
        p.drawPixmap(0,330,QPixmap(QString(":/resources/pictures/%1.png").arg(dom.right_value())));
    p.end();
    this->setPixmap(visual_domino.scaled(this->size(), Qt::IgnoreAspectRatio));
}

void VisualDominoshka::mousePressEvent(QMouseEvent *event)
{
    //this->move(this->x(), this->y() - 50);
    emit mouse_clicked(index);
    event->accept();
}

void VisualDominoshka::reduce_index()
{
    index--;
    this->move(this->x()-50, 690);
}

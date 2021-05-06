#ifndef VISUALDOMINOSHKA_H
#define VISUALDOMINOSHKA_H

#include <QLabel>
#include <QMouseEvent>
#include "dominoshka.h"

class VisualDominoshka : public QLabel
{
    Q_OBJECT
public:
    explicit VisualDominoshka(int i, Dominoshka& dom, QWidget *parent = nullptr);
    void reduce_index();

signals:
    void mouse_clicked(int index);

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    int index;
};

#endif // VISUALDOMINOSHKA_H

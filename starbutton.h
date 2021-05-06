#ifndef STARBUTTON_H
#define STARBUTTON_H

#include <QLabel>
#include <QMouseEvent>

class StarButton : public QLabel
{
    Q_OBJECT
public:
    explicit StarButton(char aspect, QWidget *parent = nullptr);

signals:
    void star_clicked(char aspect);

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    char side;
};

#endif // STARBUTTON_H

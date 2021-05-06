#ifndef DOMINO_H
#define DOMINO_H

#include "hands.h"
#include "visualdominoshka.h"
#include "starbutton.h"

#include <QMainWindow>

#include <QList>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class Domino; }
QT_END_NAMESPACE

typedef struct Corner
{
    int x, y, value;
    char direction;
    bool horizontal;
    StarButton* star;
} Corner;

class Domino : public QMainWindow
{
    Q_OBJECT

public:
    Domino(QWidget *parent = nullptr);
    ~Domino();
    void put_on_table(char side, int player, int index);
    void skip();
    void bot_turn(int index);
    void finish(int index);
    void get_bank(int index);

private slots:
    void on_BankButton_clicked();
    void chose_domino(int index);
    void set_domino(char side);

private:
    Ui::Domino *ui;

    Corner left = {400, 0, -1, 'l', true, nullptr}, right = {500, 0, -1, 'r', true, nullptr};
    QString nick;
    int players, first = 0, skip_count = 0;
    Hands *players_hands, bank;
    QList <VisualDominoshka *> own_dominoes;
    QList <QLabel *> bots;

    int cur_dom = -1;
};
#endif // DOMINO_H

#include "domino.h"
#include "ui_domino.h"

#include <QString>
#include <QMessageBox>

#include <QPixmap>
#include <QPainter>

#include "startmenu.h"

#include <QDebug>

Domino::Domino(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Domino)
{
    ui->setupUi(this);

    //StartMenu starter(&nick, &players);
    //starter.exec();
    //ui->NickName->setText(nick);

    players = 2;

    if (players != 2)
        this->setMaximumWidth(1000);

    this->setWindowIcon(QIcon(":/resources/pictures/domino.png"));
    ui->table->setPixmap(QPixmap(":/resources/pictures/table.jpg").scaled(ui->table->size(), Qt::IgnoreAspectRatio));
    right.star = new StarButton('r', this);
    left.star = new StarButton('l', this);

    QObject::connect(right.star, SIGNAL(star_clicked(char)), this, SLOT(set_domino(char)));
    QObject::connect(left.star, SIGNAL(star_clicked(char)), this, SLOT(set_domino(char)));

    srand(static_cast<unsigned int>(time(0)));

    players_hands = new Hands[players];

    int i = 0, j = 0, index = 0, min_dom = 7, min_index = 0;

    for (i = 0; i < 7; i++)
        for (j = i; j < 7; j++)
            bank.add(Dominoshka(i, j));

    QFont font = ui->NickName->font();
    font.setPixelSize(15);
    for (i = 0; i < players; i++)
    {
        if (i)
        {
            bots << new QLabel(QString("Бот №%1 [7]").arg(i), this);
            bots[i-1]->setFont(ui->NickName->font());
            bots[i-1]->setFont(font);
            bots[i-1]->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            ui->bot_board->addWidget(bots[i-1]);
        }

        for (j = 0; j < 7; j++)
        {
            index = rand() % bank.length();
            if ((!bank.at(index).rotation_value()) && (bank.at(index).left_value()) && (bank.at(index).left_value() < min_dom))
            {
                first = i;
                min_index = j;
                min_dom = bank.at(index).left_value();
            }
            players_hands[i].add(bank.at(index));
            bank.del(index);
        }
    }

    if (min_dom != 7)
    {
        put_on_table('l', first, min_index);
        left.value = players_hands[first].at(min_index).left_value();
        right.value = players_hands[first].at(min_index).right_value();
        players_hands[first].del(min_index);

        if (first)
            bots[first - 1]->setText(QString("Бот №%1 [6]").arg(first));

        first = (first + 1) % players;
    }

    if (first)
        for (int i = first; i < players; i++)
            bot_turn(i);

    for (i = 0; i < players_hands[0].length(); i++)
    {
        own_dominoes << new VisualDominoshka(i, players_hands[0].at(i), this);
        QObject::connect(own_dominoes[own_dominoes.length()-1], SIGNAL(mouse_clicked(int)), this, SLOT(chose_domino(int)));
    }

    if (players != 4)
    {
        ui->BankButton->setText(QString("Банк: %1").arg(bank.length()));
        ui->BankButton->setEnabled(true);
    }

    //QMessageBox::information(this, "Checking", QString("Переменная: %1").arg(dominos.left_value()));
    //qDebug() << "Ok: \t" << index;
}

Domino::~Domino()
{
    delete ui;
    delete [] players_hands;
    own_dominoes.clear();
    bots.clear();
}

void Domino::on_BankButton_clicked()
{
    if (ui->BankButton->text() == "Пропуск хода")
        skip();
    else
    {
        get_bank(0);

        own_dominoes << new VisualDominoshka(own_dominoes.length(), players_hands[0].at(own_dominoes.length()), this);
        QObject::connect(own_dominoes[own_dominoes.length()-1], SIGNAL(mouse_clicked(int)), this, SLOT(chose_domino(int)));
        own_dominoes[own_dominoes.length()-1]->show();

        if (players == 2)
            if (players_hands[0].length() > 14)
                this->resize(this->width() + 50, this->height());
    }
}

void Domino::chose_domino(int index)
{
    if (cur_dom != -1)
        own_dominoes[cur_dom]->move(own_dominoes[cur_dom]->x(), own_dominoes[cur_dom]->y() + 25);
    cur_dom = index;
    own_dominoes[cur_dom]->move(own_dominoes[cur_dom]->x(), own_dominoes[cur_dom]->y() - 25);
    if ((right.value == players_hands[0].at(index).left_value()) || (right.value == players_hands[0].at(index).right_value()))
        right.star->show();
    else
        right.star->hide();
    if ((left.value == players_hands[0].at(index).left_value()) || (left.value == players_hands[0].at(index).right_value()))
        left.star->show();
    else
        left.star->hide();
}

void Domino::put_on_table(char side, int player, int index)
{
    QPixmap visual_table(ui->table->pixmap());
    QPainter painter(&visual_table);

    QPixmap stone(":/resources/pictures/BaseDom.png");
    QPainter p(&stone);
    if (players_hands[player].at(index).left_value() != 0)
        p.drawPixmap(0,0,QPixmap(QString(":/resources/pictures/%1.png").arg(players_hands[player].at(index).left_value())));
    if (players_hands[player].at(index).right_value() != 0)
        p.drawPixmap(0,330,QPixmap(QString(":/resources/pictures/%1.png").arg(players_hands[player].at(index).right_value())));
    p.end();
    stone = stone.scaled(45, 95, Qt::IgnoreAspectRatio);

    if (side == 'r')
        switch (right.direction)
        {
        case 'u':
            if ((!players_hands[player].at(index).rotation_value()) && (!right.horizontal))
            {
                right.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(right.x - 22, right.y+50, rotation.transformed(trans));
                right.y -= 50;
            }
            else
            {
                right.horizontal = false;
                painter.drawPixmap(right.x, right.y, stone);
                right.y -= 100;
            }

            if (right.y < 0)
            {
                if ((right.y + 50 < 0) || (right.horizontal))
                {
                    right.y += 100;
                    right.x += 50 + 23 * right.horizontal;
                }
                else
                    right.y += 28;
                right.direction = 'r';
            }
            break;

        case 'l':
            if ((!players_hands[player].at(index).rotation_value()) && (right.horizontal))
            {
                right.horizontal = false;
                painter.drawPixmap(right.x + 50, right.y - 22, stone);
                right.x -= 50;
            }
            else
            {
                right.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(90);
                painter.drawPixmap(right.x, right.y, rotation.transformed(trans));
                right.x -= 100;
            }

            if (right.x < 25)
            {
                if ((right.x + 50 < 25) || (!right.horizontal))
                {
                    right.x += 100;
                    right.y -= 100 + 22 * (!right.horizontal);
                }
                else
                {
                    right.x += 50;
                    right.y -= 50 + 22 * (!right.horizontal);
                }
                right.direction = 'u';
            }
            break;

        case 'd':
            if ((!players_hands[player].at(index).rotation_value()) && (!right.horizontal))
            {
                right.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(right.x - 22, right.y, rotation.transformed(trans));
                right.y += 50;
            }
            else
            {
                right.horizontal = false;
                painter.drawPixmap(right.x, right.y, stone);
                right.y += 100;
            }

            if (right.y + 100 > 615)
            {
                if ((right.y + 50 > 615) || (right.horizontal))
                {
                    right.x -= 50;
                    if (right.horizontal)
                    {
                        right.x -= 72;
                        right.y -= 50;
                    }
                }
                else
                    right.x -= 50;
                right.direction = 'l';
            }
            break;

        default:
            if ((!players_hands[player].at(index).rotation_value()) && (right.horizontal))
            {
                right.horizontal = false;
                painter.drawPixmap(right.x, right.y, stone);
                right.x += 50;
            }
            else
            {
                right.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(right.x, right.y + 22, rotation.transformed(trans));
                right.x += 100;
            }

            if (right.x + 100 > 975)
            {
                if ((right.x + 50 > 975) || (!right.horizontal))
                {
                    right.x -= 50;
                    right.y += 72 + 28 * (!right.horizontal);
                }
                else
                    right.y += 22;
                right.direction = 'd';
            }
            break;
        }
    else
        switch (left.direction)
        {
        case 'u':
            if ((!players_hands[player].at(index).rotation_value()) && (!left.horizontal))
            {
                left.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(left.x - 22, left.y + 50, rotation.transformed(trans));
                left.y -= 50;
            }
            else
            {
                left.horizontal = false;
                painter.drawPixmap(left.x, left.y, stone);
                left.y -= 100;
            }

            if (left.y < 0)
            {
                if ((left.y + 50 < 0) || (left.horizontal))
                {
                    left.y += 78;
                    left.x -= 100 + 23 * left.horizontal;
                }
                else
                {
                    left.y += 28;
                    left.x -= 50;
                }
                left.direction = 'l';
            }
            break;

        case 'r':
            if ((!players_hands[player].at(index).rotation_value()) && (left.horizontal))
            {
                left.horizontal = false;
                painter.drawPixmap(left.x, left.y - 22, stone);
                left.x += 50;
            }
            else
            {
                left.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(90);
                painter.drawPixmap(left.x, left.y, rotation.transformed(trans));
                left.x += 100;
            }

            if (left.x + 100 > 975)
            {
                if ((left.x + 50 > 975) || (!left.horizontal))
                {
                    left.x -= 50;
                    left.y -= 100 + 22 * (!left.horizontal); // + 28
                }
                else
                    left.y -= 50;
                left.direction = 'u';
            }
            break;

        case 'd':
            if ((!players_hands[player].at(index).rotation_value()) && (!left.horizontal))
            {
                left.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(left.x - 22, left.y, rotation.transformed(trans));
                left.y += 50;
            }
            else
            {
                left.horizontal = false;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(180);
                painter.drawPixmap(left.x, left.y, rotation.transformed(trans));
                left.y += 100;
            }

            if (left.y + 100 > 615)
            {
                if ((left.y + 50 > 615) || (left.horizontal))
                {
                    left.x += 50 + 28 * left.horizontal;
                    left.y -= 50;
                }
                left.direction = 'r';
            }
            break;

        default:
            if ((!players_hands[player].at(index).rotation_value()) && (left.horizontal))
            {
                left.horizontal = false;
                painter.drawPixmap(left.x + 50, left.y, stone);
                left.x -= 50;
            }
            else
            {
                left.horizontal = true;
                QPixmap rotation(stone);
                QTransform trans;
                trans.rotate(-90);
                painter.drawPixmap(left.x, left.y + 22, rotation.transformed(trans));
                left.x -= 100;
            }

            if (left.x < 25)
            {
                if ((left.x + 50 < 25) || (!left.horizontal))
                {
                    left.x += 50;
                    left.y += 72 + 28 * (!left.horizontal);
                }
                else
                    left.y += 22;
                left.x += 50;
                left.direction = 'd';
            }
            break;
        }

    painter.end();
    ui->table->setPixmap(visual_table.scaled(ui->table->size(), Qt::IgnoreAspectRatio));
}

void Domino::set_domino(char side)
{
    right.star->hide();
    left.star->hide();

    if (skip_count)
        skip_count = 0;

    if (side == 'l')
    {
        if (left.value != players_hands[0].at(cur_dom).right_value())
            players_hands[0].at(cur_dom).swap_value();
        left.value = players_hands[0].at(cur_dom).left_value();
    }
    else
    {
        if (right.value != players_hands[0].at(cur_dom).left_value())
            players_hands[0].at(cur_dom).swap_value();
        right.value = players_hands[0].at(cur_dom).right_value();
    }
    put_on_table(side, 0, cur_dom);

    delete own_dominoes[cur_dom];
    own_dominoes.removeAt(cur_dom);
    for (int i = cur_dom; i < own_dominoes.length(); i++)
        own_dominoes[i]->reduce_index();
    players_hands[0].del(cur_dom);
    cur_dom = -1;

    if (players == 2)
        if (players_hands[0].length() > 14)
            this->resize(this->width() - 50, this->height());

    if (!players_hands[0].length())
    {
        finish(0);
        return;
    }

    for (int i = 1; i < players; i++)
        bot_turn(i);
}

void Domino::skip()
{
    skip_count++;
    if (skip_count == players)
        QMessageBox::warning(this, "Конец игры", "Завершение катки по скипам");
}

void Domino::bot_turn(int index)
{
    int number = rand() % 2;
    char side;
    if (number)
    {
        side = 'l';
        number = left.value;
    }
    else
    {
        side = 'r';
        number = right.value;
    }

    for (int i = 0; i < players_hands[index].length(); i++)
        if ((players_hands[index].at(i).left_value() == number) || (players_hands[index].at(i).right_value() == number))
        {
            if (side == 'l')
            {
                if (left.value != players_hands[index].at(i).right_value())
                    players_hands[index].at(i).swap_value();
                left.value = players_hands[index].at(i).left_value();
            }
            else
            {
                if (right.value != players_hands[index].at(i).left_value())
                    players_hands[index].at(i).swap_value();
                right.value = players_hands[index].at(i).right_value();
            }
            put_on_table(side, index, i);
            bots.at(index-1)->setText(QString("Бот №%1 [%2]\nХод: %3/%4").arg(index).arg(players_hands[index].length()-1).
                                      arg(players_hands[index].at(i).left_value()).arg(players_hands[index].at(i).right_value()));
            players_hands[index].del(i);
            skip_count = 0;
            if (!players_hands[index].length())
                finish(index);
            return;
        }

    if (side == 'r')
    {
        side = 'l';
        number = left.value;
    }
    else
    {
        side = 'r';
        number = right.value;
    }

    for (int i = 0; i < players_hands[index].length(); i++)
        if ((players_hands[index].at(i).left_value() == number) || (players_hands[index].at(i).right_value() == number))
        {
            if (side == 'l')
            {
                if (left.value != players_hands[index].at(i).right_value())
                    players_hands[index].at(i).swap_value();
                left.value = players_hands[index].at(i).left_value();
            }
            else
            {
                if (right.value != players_hands[index].at(i).left_value())
                    players_hands[index].at(i).swap_value();
                right.value = players_hands[index].at(i).right_value();
            }
            put_on_table(side, index, i);
            bots.at(index-1)->setText(QString("Бот №%1 [%2]\nХод: %3/%4").arg(index).arg(players_hands[index].length()-1)
                                      .arg(players_hands[index].at(i).left_value()).arg(players_hands[index].at(i).right_value()));
            players_hands[index].del(i);
            skip_count = 0;
            if (!players_hands[index].length())
                finish(index);
            return;
        }

    if (bank.length())
    {
        Dominoshka* last_dom = &players_hands[index].at(players_hands[index].length()-1);
        while ((bank.length()) && ((left.value != last_dom->left_value()) && (left.value != last_dom->right_value())
                                    && (right.value != last_dom->left_value()) && (right.value != last_dom->right_value())))
        {
            get_bank(index);
            last_dom = &players_hands[index].at(players_hands[index].length()-1);
        }
        if ((left.value == last_dom->left_value())|| (left.value == last_dom->right_value())
                || (right.value == last_dom->left_value()) || (right.value == last_dom->right_value()))
        {
            if ((left.value == last_dom->left_value()) || (left.value == last_dom->right_value()))
            {
                side = 'l';
                if (left.value != last_dom->right_value())
                     players_hands[index].at(players_hands[index].length()-1).swap_value();
                left.value = players_hands[index].at(players_hands[index].length()-1).left_value();
            }
            else
            {
                side = 'r';
                if (right.value != last_dom->left_value())
                     players_hands[index].at(players_hands[index].length()-1).swap_value();
                right.value = players_hands[index].at(players_hands[index].length()-1).right_value();
            }
            last_dom = &players_hands[index].at(players_hands[index].length()-1);
            put_on_table(side, index, players_hands[index].length()-1);
            bots.at(index-1)->setText(QString("Бот №%1 [%2]\nХод: %3/%4").arg(index).arg(players_hands[index].length()-1)
                                      .arg(last_dom->left_value()).arg(last_dom->right_value()));
            players_hands[index].del(players_hands[index].length()-1);
            skip_count = 0;
            return;
        }
    }

    bots.at(index-1)->setText(QString("Бот №%1 [%2]\nПропуск хода").arg(index).arg(players_hands[index].length()));
    skip();
}

void Domino::finish(int index)
{
    QMessageBox::information(this, "Завершение", "Победа!!");
}

void Domino::get_bank(int index)
{
    int number = rand() % bank.length();
    players_hands[index].add(bank.at(number));
    bank.del(number);

    if (!bank.length())
        ui->BankButton->setText("Пропуск хода");
    else
        ui->BankButton->setText(QString("Банк: %1").arg(bank.length()));
}

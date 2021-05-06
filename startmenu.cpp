#include "startmenu.h"
#include "ui_startmenu.h"

#include <QCloseEvent>
#include <QMessageBox>

StartMenu::StartMenu(QString *name, int *players, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);

    NickName = name;
    this->players = players;

    this->setWindowIcon(QIcon(":/resources/pictures/domino.png"));
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::closeEvent(QCloseEvent *my_event)
{
    my_event->accept();
    exit(0);
}

void StartMenu::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "Информация", "\tКурсовая работа по ЯП: Домино 'Морской козёл' на QT\n\tМ8О-111Б-20: Комиссаров Антон\n\n"
"\tШаги для начала игры:\n1) Введите NickName.\n2) Выберите количество игроков.\n3) Нажмите кнопку 'Играть'.");
}

void StartMenu::on_pushButton_clicked()
{
    if (ui->lineEdit->text().length() == 0)
        QMessageBox::critical(this, "Ошибка", "Введите свой NickName!");
    else if (!(ui->radioButton->isChecked() || ui->radioButton_2->isChecked() || ui->radioButton_3->isChecked()))
        QMessageBox::critical(this, "Ошибка", "Выберите количество противников!");
    else
    {
        *NickName = ui->lineEdit->text();
        if (ui->radioButton->isChecked())
            *players = 2;
        else if (ui->radioButton_2->isChecked())
            *players = 3;
        else
            *players = 4;
        QApplication::exit();
    }
}

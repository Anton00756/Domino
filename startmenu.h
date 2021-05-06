#ifndef STARTMENU_H
#define STARTMENU_H

#include <QDialog>

namespace Ui {
class StartMenu;
}

class StartMenu : public QDialog
{
    Q_OBJECT

public:
    explicit StartMenu(QString *name, int *players, QWidget *parent = nullptr);
    ~StartMenu();
    void closeEvent(QCloseEvent *my_event);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    QString *NickName;
    int *players;

    Ui::StartMenu *ui;
};

#endif // STARTMENU_H

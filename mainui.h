#ifndef MAINUI_H
#define MAINUI_H

#include <QDialog>

namespace Ui {
class MainUI;
}

class MainUI : public QDialog
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

private:
    Ui::MainUI *ui;
};

#endif // MAINUI_H

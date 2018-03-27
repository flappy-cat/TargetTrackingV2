#include "mainui.h"
#include "ui_mainui.h"

MainUI::MainUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
}

MainUI::~MainUI()
{
    delete ui;
}

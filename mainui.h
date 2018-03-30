#ifndef MAINUI_H
#define MAINUI_H

#include <QDialog>
#include"globalsettings.h"
#include"managerofcommunicaion.h"
#include "videoProcService/videoencodeh264.h"
#include"udpService/serviceudpvideo.h"
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
    bool isWorking;
    QPushButton* btn_StartOrStop;

public slots:
    void startorstopWorking();
private:
    ServiceUDPVideo         udpThread;
    VideoEncodeH264         encThread;
    GlobalSettings*         pSettings;
    ManagerOfCommunicaion*  pComManager;
};

#endif // MAINUI_H

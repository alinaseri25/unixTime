#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

typedef struct
{
    uint8_t second; // 0-59
    uint8_t minute; // 0-59
    uint8_t hour;   // 0-23
    uint8_t day;    // 1-31
    uint8_t month;  // 1-12
    uint16_t year;   // 0 - 65535 year
}
date_time_t;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Btn_Convert_To_Unix_clicked();

    void on_Btn_Convert_To_Real_clicked();

private:
    Ui::MainWindow *ui;

    uint16_t days[4][12] =
    {
        {   0,  31,  60,  91, 121, 152, 182, 213, 244, 274, 305, 335},
        { 366, 397, 425, 456, 486, 517, 547, 578, 609, 639, 670, 700},
        { 731, 762, 790, 821, 851, 882, 912, 943, 974,1004,1035,1065},
        {1096,1127,1155,1186,1216,1247,1277,1308,1339,1369,1400,1430},
    };

    uint64_t date_time_to_epoch(date_time_t* date_time,bool IsUnixTime = true);
    void epoch_to_date_time(date_time_t* date_time, uint64_t t,bool IsUnixTime = true);

    date_time_t curtime;
    QDateTime datetime;
};

#endif // MAINWINDOW_H

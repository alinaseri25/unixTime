#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Unix Time System"));
    uint8_t DateTime[100];
    curtime.day = 17;
    curtime.month = 05;
    curtime.year = 2100;

    curtime.hour = 20;
    curtime.minute = 41;
    curtime.second = 10;

    uint64_t epc = date_time_to_epoch(&curtime,false);

    sprintf((char *)DateTime,"%04d/%02d/%02d %02d:%02d:%02d",curtime.year,curtime.month,curtime.day
            ,curtime.hour,curtime.minute,curtime.second);
    ui->Txt_Output_data->append(QString("Current Date Time : %1").arg(QString::fromLatin1((char *)DateTime)));
    ui->Txt_Output_data->append(QString("Current Unix Time : %1").arg(epc));
    ui->Txt_Output_data->append(QString("\\*--------------------------*/"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

uint64_t MainWindow::date_time_to_epoch(date_time_t *date_time, bool IsUnixTime)
{
    uint16_t y;
    uint16_t m;
    uint16_t d;
    uint64_t t;

    //Year
    y = date_time->year;
    //Month of year
    m = date_time->month;
    //Day of month
    d = date_time->day;

    //January and February are counted as months 13 and 14 of the previous year
    if(m <= 2)
    {
       m += 12;
       y -= 1;
    }

    //Convert years to days
    t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
    //Convert months to days
    t += (30 * m) + (3 * (m + 1) / 5) + d;
    if(IsUnixTime)
    {
        //Unix time starts on January 1st, 1970
        t -= 719561;
    }
    else
    {
        //Unix time starts on January 1st, 1900
        t -= 693994;
    }
    //Convert days to seconds
    t *= 86400;
    //Add hours, minutes and seconds
    t += (3600 * date_time->hour) + (60 * date_time->minute) + date_time->second;

    //Return Unix time
    return t;
}

void MainWindow::epoch_to_date_time(date_time_t *date_time, uint64_t t, bool IsUnixTime)
{
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t d;
    uint32_t e;
    uint32_t f;

    //Negative Unix time values are not supported
    if(t < 1)
       t = 0;

    //Retrieve hours, minutes and seconds
    date_time->second = t % 60;
    t /= 60;
    date_time->minute = t % 60;
    t /= 60;
    date_time->hour = t % 24;
    t /= 24;

    //Convert Unix time to date
    a = (uint32_t) ((4 * t + 102032) / 146097 + 15);
    b = (uint32_t) (t + 2442113 + a - (a / 4));
    c = (20 * b - 2442) / 7305;
    d = b - 365 * c - (c / 4);
    e = d * 1000 / 30601;
    f = d - e * 30 - e * 601 / 1000;

    //January and February are counted as months 13 and 14 of the previous year
    uint32_t TimeToMinus;
    if(IsUnixTime)
    {
        //Unix time starts on January 1st, 1970
        TimeToMinus = 4716;
    }
    else
    {
        //Unix time starts on January 1st, 1900
        TimeToMinus = 4786;
    }
    if(e <= 13)
    {
       c -= TimeToMinus;
       e -= 1;
    }
    else
    {
       c -= (TimeToMinus - 1);
       e -= 13;
    }

    //Retrieve year, month and day
    date_time->year = c;
    date_time->month = e;
    date_time->day = f;
}

void MainWindow::on_Btn_Convert_To_Unix_clicked()
{
    datetime = QDateTime::currentDateTime();
    curtime.day = datetime.date().day();
    curtime.month = datetime.date().month();
    curtime.year = datetime.date().year();// - 2000

    curtime.second = datetime.time().second();
    curtime.minute = datetime.time().minute();
    curtime.hour = datetime.time().hour();//200;//

    uint32_t epc = date_time_to_epoch(&curtime); //611447308

    ui->Txt_Output_data->append(QString("Current Date Time : %1").arg(datetime.toString(QString("yyyy/MM/dd  HH:mm:ss"))));
    ui->Txt_Output_data->append(QString("Current Unix Time : %1").arg(epc));
    ui->Txt_Output_data->append(QString("\\*--------------------------*/"));
}

void MainWindow::on_Btn_Convert_To_Real_clicked()
{
    uint64_t epc;
    QString curStr = ui->Txt_Input_Data->text();
    uint8_t DateTime[100];
    epc = curStr.toULongLong();
    epoch_to_date_time(&curtime,epc);
    ui->Txt_Output_data->append(QString("Current Unix Time : %1").arg(epc));
    sprintf((char *)DateTime,"%04d/%02d/%02d %02d:%02d:%02d",curtime.year,curtime.month,curtime.day
            ,curtime.hour,curtime.minute,curtime.second);
    ui->Txt_Output_data->append(QString("Current Date Time : %1").arg(QString::fromLatin1((char *)DateTime)));
    ui->Txt_Output_data->append(QString("\\*--------------------------*/"));
}

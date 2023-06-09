void on_enter_clicked(bool checked);
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include <QSerialPort>
#include <QString>
#include <QTimer>
#include <QSerialPortInfo>//串口信息
#include <QMessageBox>
#include <QDebug>
#include<QTextCodec>
#include <QDir>
#include <QFileDialog>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    QSerialPort *serialPort;//定义串口指针

private slots:

    void UpdatePort();
    /*手动连接槽函数*/
    void manual_serialPortReadyRead();


    void on_openport_clicked();

    void on_closeport_clicked();

    void on_send_clicked();

    void on_clear_clicked();


    void on_messagebox_textChanged();


    void on_savefile_clicked();

    void on_readfile_clicked();

private:
    Ui::Widget *ui;
    QStringList serialNamePort;
    QTimer *timer;

};
#endif // WIDGET_H

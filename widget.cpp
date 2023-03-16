#include "widget.h"
#include "ui_widget.h"
#include "QSerialPortInfo"
#include <QSerialPort>
#include <QMessageBox>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 定义窗口名字
    this->setWindowTitle("串口助手");
//    this->setWindowIcon(QIcon(":/icon.jpg"));

    serialPort = new QSerialPort(this);



    /*找出当前连接的串口*/
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        serialNamePort.append(info.portName());
    }
    // 找出当前连接的串口
    ui->portnumber->addItems(serialNamePort);

    // 定时更新串口
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Widget::UpdatePort);
    timer->start(1000);

    // 读取数据
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(manual_serialPortReadyRead()));/*手动连接槽函数*/

}

Widget::~Widget()
{
    delete ui;
}


//更新串口
void Widget::UpdatePort()
{
    QStringList newportList;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        newportList<<info.portName();
    }
    if(newportList.size()>serialNamePort.size())
    {
        serialNamePort=newportList;
        ui->portnumber->clear();
        ui->portnumber->addItems(serialNamePort);
        qDebug()<<"更新串口增加";
    }else if(newportList.size()<serialNamePort.size()){

        serialNamePort=newportList;
        ui->portnumber->clear();
        ui->portnumber->addItems(serialNamePort);
        qDebug()<<"更新串口减少";
//        on_openport_clicked();
    }
}


/*手动实现接收数据函数，接收数据*/
void Widget::manual_serialPortReadyRead()
{
    QByteArray recbuf;
    int recnum = 0;
    recbuf = serialPort->readAll();
    recnum = recbuf.size();
    qDebug("接收到的数据长度:%d",recnum);
    qDebug()<<"格式化16进制小写输出"<<QString().sprintf("%04x",recbuf);
//    ui->messagebox->insertPlainText(QString::fromUtf8(serialPort->readAll()));
    ui->messagebox->insertPlainText(recbuf);
}





void Widget::on_openport_clicked()
{
    /*串口初始化*/
    //初始化设置，要设置的
    QSerialPort::BaudRate baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::StopBits stopBits;
    QSerialPort::Parity checkBits;

    //获取设置的波特率
    if(ui->baudrate->currentText()=="4800")
        baudRate=QSerialPort::Baud4800;
    else if(ui->baudrate->currentText()=="9600")
        baudRate=QSerialPort::Baud9600;
    else if(ui->baudrate->currentText()=="115200")
        baudRate=QSerialPort::Baud115200;

    // 获取设置的数据位
    if(ui->databit->currentText()=="5")
        dataBits=QSerialPort::Data5;
    else if(ui->databit->currentText()=="6")
        dataBits=QSerialPort::Data6;
    else if(ui->databit->currentText()=="7")
        dataBits=QSerialPort::Data7;
    else if(ui->databit->currentText()=="8")
        dataBits=QSerialPort::Data8;

    // 获取设置的停止位
    if(ui->stopbit->currentText()=="1")
        stopBits=QSerialPort::OneStop;
    else if(ui->stopbit->currentText()=="1.5")
        stopBits=QSerialPort::OneAndHalfStop;
    else if(ui->stopbit->currentText()=="2")
        stopBits=QSerialPort::TwoStop;

    //获取设置的校验位
    if(ui->checkbit->currentText()=="none")
        checkBits=QSerialPort::NoParity;

    // 设置参数
    serialPort->setPortName(ui->portnumber->currentText());
//    serialPort->setStopBits(stopBits);
//    serialPort->setParity(checkBits);
    serialPort->setBaudRate(baudRate);
    switch(dataBits){
        case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        default:
            serialPort->setDataBits(QSerialPort::UnknownDataBits);
            break;
     }

    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    // 无流控制
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if(serialPort->open(QIODevice::ReadWrite)==true)
        QMessageBox::information(this,"提示","打开串口成功");
    else
        QMessageBox::critical(this,"提示","打开串口失败");
}

// 关闭串口
void Widget::on_closeport_clicked()
{
    serialPort->close();
    QMessageBox::information(this,"提示","关闭串口");
}

// 发送数据
void Widget::on_send_clicked()
{
    long long a = 0;
    QString sendstr = ui->sendbox->toPlainText();
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QByteArray bytes = codec->fromUnicode(sendstr);
    bytes = sendstr.toLocal8Bit();
    // 对发送数据追加回车
    bytes.append("\r\n");
    a= serialPort->write(bytes.data());
    qDebug("本次数据发送长度:%d",a);


    //每次发送清空发送区域
//    ui->sendbox->clear();
}

// 清除接收窗口
void Widget::on_clear_clicked()
{
    ui->messagebox->clear();
}

// 接收窗口自动滚动
void Widget::on_messagebox_textChanged()
{
    ui->messagebox->moveCursor(QTextCursor::End);
}


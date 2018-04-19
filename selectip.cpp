#include "selectip.h"
#include "ui_selectip.h"

extern QString ip;                 //ip
extern QString broadcast;                  //广播地址
selectip::selectip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectip)
{
    setAttribute(Qt::WA_DeleteOnClose);             //关闭窗口后调用析构函数
    ui->setupUi(this);

    ser = NULL;
    updateipcombox();       //获得可用ip地址

}

selectip::~selectip()
{
    ser->close();
    delete ser;
    for(int i = 0;i<cli.count();i++){
        delete cli.at(i);
    }
    exit(1);
}

void selectip::updateipcombox()
{
    //获取所有网络接口的列表
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list) //遍历每一个网络接口
    {
        qDebug() << "Device: "<<interface.name(); //设备名
        //硬件地址
        qDebug() << "HardwareAddress: "<<interface.hardwareAddress();

        //获取IP地址条目列表，每个条目中包含一个IP地址，
        //一个子网掩码和一个广播地址
       QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)//遍历每个IP地址条目
        {
            qDebug()<<"IP Address: "<<entry.ip().toString(); //IP地址
            qDebug()<<"Netmask: "<<entry.netmask().toString(); //子网掩码
            qDebug()<<"Broadcast: "<<entry.broadcast().toString();//广播地址
            if(entry.broadcast().toString() != QString("")){
                usableip.insert(entry.ip().toString(),entry.broadcast().toString());
            }
        }
    }
    if(0 == usableip.count()){
        QMessageBox::information(this,QString("提示："),QString("无可用ip，程序即将退出！"));
        exit(1);
    }

    QList<QString> tem ;
    QMapIterator<QString,QString> i(usableip);
    while(i.hasNext()){
        i.next();
        tem.append(i.key());
    }
    ui->ipcomboBox->insertItems(0,tem);
}



void selectip::on_ipcomboBox_currentIndexChanged(const QString &arg1)
{
    ip = arg1;
    broadcast = usableip.value(ip);
}

void selectip::on_pushButton_3_clicked()
{
    if(NULL == ser){
        ser = new server(0);
        ser->setWindowTitle(QString("中控"));
        connect(ser,SIGNAL(destroyed(QObject*)),this,SLOT(newser(QObject*)));  //服务器窗口摧毁后进行
    }
    if(ser->isHidden())
        ser->show();
}

void selectip::on_pushButton_2_clicked()
{
    client *tem = new client(0,1234);
    cli.append(tem);
    tem->setWindowTitle(QString("小卫星"));
    tem->show();
}

void selectip::newser(QObject *)
{
    ser = NULL;
}


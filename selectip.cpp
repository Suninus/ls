#include "selectip.h"
#include "ui_selectip.h"

selectip::selectip(QWidget *parent,winpcap *tem) :
    QWidget(parent),
    ui(new Ui::selectip)
{
    ui->setupUi(this);
    if(tem == NULL)
        exit(1);
    arp = tem;
    updateipcombox();
}

selectip::~selectip()
{
    delete ui;
    delete arp;
}

void selectip::updateipcombox()
{
    QList<QString> tem = arp->getusableip();
    ui->ipcomboBox->insertItems(0,tem);
}


void selectip::on_pushButton_clicked()
{
    arp->getactmac();
}

void selectip::on_ipcomboBox_currentIndexChanged(const QString &arg1)
{
    ui->dev_tip->setText(QString("网络检测中"));
    qDebug()<<"ip changged";
    arp->newhandle(arg1);

    qDebug()<<arp->getselfmac().length();
    QString mac = QString("子网掩码：%1\nmac地址：%2").arg(arp->getnetmask()).arg(arp->macos());
    ui->dev_tip->setText(mac);
}

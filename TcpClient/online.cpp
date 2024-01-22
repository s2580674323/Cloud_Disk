#include "online.h"
#include "ui_online.h"

#include <QDebug>
#include <QMessageBox>
#include "tcpclient.h"
#include <QString>

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
    this->setWindowTitle("在线用户列表");
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);

    QString qss = "QWidget {"
                  "    background-color: #add8e6;"  // 偏蓝色
                  "    color: #333333;"
                  "    font-family: Arial, sans-serif;"
                  "}"

                  "QPushButton {"
                  "    background-color: #4682b4;"  // 按钮蓝色
                  "    color: white;"
                  "    border: none;"
                  "    padding: 5px 10px;"  // 增大按钮大小
                  "    font-size: 18px;"  // 指定字体大小
                  "    border-radius: 8px;"
                  "}"

                  "QPushButton:hover {"
                  "    background-color: #5f9ea0;"
                  "}";
    this->setStyleSheet(qss);


    connect(ui->addFriendBtn, &QPushButton::clicked, this, &Online::AddFriend);
    connect(ui->pushButton_Close, &QPushButton::clicked, this, &Online::close);
}

Online::~Online()
{
    delete ui;
}

void Online::ShowUsr(PDU *pdu)
{
    if(pdu == nullptr) return;
    ui->listWidget->clear();
    uint uiSize = pdu->uiMsgLen/32;
    char ctmp[32];
    for(uint i = 0; i < uiSize; ++i) {
        memcpy(ctmp, (char*)(pdu->cMsg)+i*32, 32);
        ui->listWidget->addItem(ctmp);
    }
}

void Online::AddFriend()
{
    if(ui->listWidget->currentItem() == nullptr) return;
    QString usrname = ui->listWidget->currentItem()->text();
    QString myname = TcpClient::getInstance()->getMyUsrName();
    if(usrname == myname) {
        QMessageBox::information(nullptr, "prompt", "自己不能添加自己为好友");
        return;
    }
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    // cData 中，前边是要添加的用户名字，后边是申请者的名字
    memcpy(pdu->cData, usrname.toStdString().c_str(), usrname.toStdString().size());
    memcpy(pdu->cData + 32, myname.toStdString().c_str(), myname.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = nullptr;
}

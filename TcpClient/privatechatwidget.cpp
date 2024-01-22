#include "privatechatwidget.h"
#include "ui_privatechatwidget.h"
#include "tcpclient.h"
#include <QMutex>

PrivateChatWidget::PrivateChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChatWidget)
{
    ui->setupUi(this);
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    ui->textEdit_ShowMessage->setReadOnly(true);

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


    connect(ui->pushButton_Close, &QPushButton::clicked, this, &PrivateChatWidget::close);
    connect(ui->pushButton_Min, &QPushButton::clicked, this, &PrivateChatWidget::showMinimized);

    connect(ui->pushButton_SendMessage, &QPushButton::clicked, this, &PrivateChatWidget::clickedBtnSendMessage);
}

PrivateChatWidget::~PrivateChatWidget()
{
    delete ui;
}

//PrivateChatWidget *PrivateChatWidget::self = 0;
//PrivateChatWidget *PrivateChatWidget::getInstance()
//{
//    if(self == nullptr) {
//        QMutex mutex;
//        QMutexLocker locker(&mutex);
//        if(self == nullptr) {
//            self = new PrivateChatWidget;
//        }
//    }
//    return self;
//}

QString PrivateChatWidget::getFrindName()
{
    return m_friName;
}


void PrivateChatWidget::setChatName(QString friName)
{
    m_friName = friName;
    m_myName = TcpClient::getInstance()->getMyUsrName();
    QString ShowFriendNameStr = QString("好友 : %1").arg(friName);
    ui->label_ShowFriendName->setText(ShowFriendNameStr);
}

void PrivateChatWidget::updateMsg(const PDU *pdu)
{
    if(pdu == nullptr) return;
    char SenderName[32];
    memcpy(SenderName, pdu->cData + 32, 32);
    QString strMsg = QString("%1 : %2").arg(SenderName).arg((char*)pdu->cMsg);
    ui->textEdit_ShowMessage->append(strMsg);
}

void PrivateChatWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        clickedBtnSendMessage();
        return;
    }
    QWidget::keyPressEvent(event);
}

void PrivateChatWidget::clickedBtnSendMessage()
{
    QString Message = ui->lineEdit_InputMessage->text();
    if(Message.isEmpty()) return;
    PDU *pdu = mkPDU(Message.toUtf8().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
    memcpy(pdu->cData, m_friName.toStdString().c_str(), m_friName.toStdString().size());
    memcpy(pdu->cData + 32, m_myName.toStdString().c_str(), m_myName.toStdString().size());
    memcpy(pdu->cMsg, Message.toStdString().c_str(), Message.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu);pdu = nullptr;
    ui->lineEdit_InputMessage->clear();
    QString strMsg = QString("%1 : %2").arg(m_myName).arg(Message);
    ui->textEdit_ShowMessage->append(strMsg);
}

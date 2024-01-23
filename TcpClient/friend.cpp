#include "friend.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QInputDialog>
#include <QTimer>

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    QFont font("Agency FB", 16);

    m_pShowMsgTE = new QTextEdit;
    m_pFriendListWidget = new QListWidget;
    m_pInputMsgLE = new QLineEdit;

    m_pFriendListWidget->setFont(font);
    m_pFriendListWidget->setFixedWidth(200);

    m_pShowMsgTE->setFont(font);
    m_pShowMsgTE->setReadOnly(true);

    m_pInputMsgLE->setFont(font);

    m_ShowMsgHistory = new QLabel("聊天记录");
    m_ShowFriendList = new QLabel("在线好友列表");
    m_ShowFriendList->setFont(font);
    m_ShowMsgHistory->setFont(font);


    m_pDeleteFriendBtn = new QPushButton("删除好友");
    m_pFlushFriendBtn = new QPushButton("刷新在线好友");
    m_pShowOnlineUserBtn = new QPushButton("所有在线用户");
    m_pSearchUserBtn = new QPushButton("查找用户");
    m_pSendMsgBtn = new QPushButton("发送");
    m_pPrivateChatBtn = new QPushButton("私聊");



    QFont Btnfont("Agency FB", 12);
    m_pDeleteFriendBtn->setFont(Btnfont);
    m_pFlushFriendBtn->setFont(Btnfont);
    m_pShowOnlineUserBtn->setFont(Btnfont);
    m_pSearchUserBtn->setFont(Btnfont);
    m_pSendMsgBtn->setFont(Btnfont);
    m_pPrivateChatBtn->setFont(Btnfont);



    // 右边按钮
    QVBoxLayout *pRightVLayot = new QVBoxLayout;
    pRightVLayot->addWidget(m_pDeleteFriendBtn);
    pRightVLayot->addWidget(m_pFlushFriendBtn);
    pRightVLayot->addWidget(m_pShowOnlineUserBtn);
    pRightVLayot->addWidget(m_pSearchUserBtn);
    pRightVLayot->addWidget(m_pPrivateChatBtn);


    // 好友列表
    QVBoxLayout *pFriendListLayout = new QVBoxLayout;
    pFriendListLayout->addWidget(m_ShowFriendList);
    pFriendListLayout->addWidget(m_pFriendListWidget);

    // 聊天记录
    QVBoxLayout *pShowMsgLayout = new QVBoxLayout;
    pShowMsgLayout->addWidget(m_ShowMsgHistory);
    pShowMsgLayout->addWidget(m_pShowMsgTE);

    QHBoxLayout *pMainHLayout = new QHBoxLayout;
    pMainHLayout->addLayout(pShowMsgLayout);
    pMainHLayout->addLayout(pFriendListLayout);
    pMainHLayout->addLayout(pRightVLayot);

    // 发送信息水平布局
    QHBoxLayout *pMsgHBLayout = new QHBoxLayout;
    pMsgHBLayout->addWidget(m_pInputMsgLE);
    pMsgHBLayout->addWidget(m_pSendMsgBtn);

    m_pOnline = new Online;

    QVBoxLayout *pMain = new QVBoxLayout;
    pMain->addLayout(pMainHLayout);
    pMain->addLayout(pMsgHBLayout);
    m_pOnline->hide();

    this->setLayout(pMain);

    connect(m_pShowOnlineUserBtn, &QPushButton::clicked, this, &Friend::clickedShowOnlineBtn);
    connect(m_pSearchUserBtn, &QPushButton::clicked, this, &Friend::clickedSearchUserBtn);
    connect(m_pFlushFriendBtn, &QPushButton::clicked, this, &Friend::clickedFlushFriendBtn);
    connect(m_pDeleteFriendBtn, &QPushButton::clicked, this, &Friend::clickedDeleteFriendBtn);
    connect(m_pPrivateChatBtn, &QPushButton::clicked, this, &Friend::clickedPrivateChatBtn);
    connect(m_pSendMsgBtn, &QPushButton::clicked, this, &Friend::clickedGroupChatBtn);

    // 每5秒刷新一次好友状态
//    QTimer *timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, this, &Friend::clickedFlushFriendBtn);    // 关联定时器的信号到Widget的槽函数上
//    timer->start(5000);  // 设置溢出时间为1s，并启动定时器
    connect(TcpClient::getInstance(), &TcpClient::flushScreenSignal, this, &Friend::clickedFlushFriendBtn);
}

void Friend::showAllOnlineUsr(PDU *pdu)
{
    if(pdu == nullptr) return;
    m_pOnline->ShowUsr(pdu);
}

void Friend::updateFriendList(PDU *pdu)
{
    if(pdu == nullptr) return;
    // 刷新前先清空
    m_pFriendListWidget->clear();

    uint uiSize = pdu->uiMsgLen/32;
    char ctmp[32];
    for(uint i = 0; i < uiSize; ++i) { // 每一个元素是QString
        memcpy(ctmp, (char*)(pdu->cMsg)+i*32, 32);
        m_pFriendListWidget->addItem(ctmp);
    }
}

void Friend::updateGroupMsg(PDU *pdu)
{
    QString str = QString("%1 : %2").arg(pdu->cData).arg((char*)pdu->cMsg);
    m_pShowMsgTE->append(str);
}

QList<PrivateChatWidget *> &Friend::getChatList()
{
    return m_pChatList;
}

QMap<QString, bool> &Friend::getVisMap()
{
    return m_vis;
}

QListWidget *Friend::getFriendList()
{
    return m_pFriendListWidget;
}


void Friend::clickedShowOnlineBtn()
{
    if(m_pOnline->isHidden()) {
        m_pOnline->show();
        //
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_REQUEST;
        TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
        free(pdu);
        pdu = nullptr;
    }
    else {
        m_pOnline->hide();
    }
}

void Friend::clickedSearchUserBtn()
{
    m_SearchName = QInputDialog::getText(this, "搜索", "用户名");
    if(m_SearchName.isEmpty()) return;
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_REQUEST;
    memcpy(pdu->cData, (char*)m_SearchName.toStdString().c_str(), STR_MAX_SIZE);
    TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = nullptr;
}

void Friend::clickedFlushFriendBtn()
{
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST;
    QString myName = TcpClient::getInstance()->getMyUsrName();
    memcpy(pdu->cData, myName.toStdString().c_str(), myName.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = nullptr;
}

void Friend::clickedDeleteFriendBtn()
{
    if(m_pFriendListWidget->currentItem() == nullptr) return;
    QString friName = m_pFriendListWidget->currentItem()->text();
    int res = QMessageBox::information(this, "询问", QString("您确定要删除好友:%1吗").arg(friName),
                             QMessageBox::Yes, QMessageBox::No);
    if(res == QMessageBox::Yes) {
        QString myName = TcpClient::getInstance()->getMyUsrName();
        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
        memcpy(pdu->cData, friName.toStdString().c_str(), friName.toStdString().size());
        memcpy(pdu->cData + STR_MAX_SIZE, myName.toStdString().c_str(), myName.toStdString().size());
        TcpClient::getInstance()->getTcpSocket()->write((char *)pdu, pdu->uiPDULen);
        free(pdu);pdu = nullptr;
    }
}

void Friend::clickedPrivateChatBtn()
{
    if(m_pFriendListWidget->currentItem() == nullptr) return;
    QString friName = m_pFriendListWidget->currentItem()->text();
    if(m_vis.contains(friName) && m_vis.value(friName) == true) {
        for(int i = 0; i < m_pChatList.count(); ++i) {
            if(m_pChatList.at(i)->getFrindName() == friName) {
                m_pChatList.at(i)->show();
            }
        }
        return;
    }
    m_vis.insert(m_vis.end(),friName, true);
    // 新建一个私聊对话框，myName 用户向 friName 发起私聊
    PrivateChatWidget *m_pNewPrivateChat = new PrivateChatWidget;
    m_pNewPrivateChat->setChatName(friName);
    m_pNewPrivateChat->setWindowTitle(QString("与%1的聊天框").arg(friName));
    m_pChatList.append(m_pNewPrivateChat);
    m_pNewPrivateChat->show();
//    PrivateChatWidget::getInstance()->setChatName(friName); // 设置私聊双方名字
//    PrivateChatWidget::getInstance()->setWindowTitle(QString("好友 : %1").arg(friName));
//    if(PrivateChatWidget::getInstance()->isHidden()) {
//        PrivateChatWidget::getInstance()->show();
//    }
}

void Friend::clickedGroupChatBtn()
{
    QString Message = m_pInputMsgLE->text();
    m_pInputMsgLE->clear();
    if(Message.isEmpty()) return;
    QString myName = TcpClient::getInstance()->getMyUsrName();

    PDU *pdu = mkPDU(Message.toUtf8().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_GROUP_CHAT_REQUEST;
    memcpy(pdu->cData, myName.toStdString().c_str(), myName.toStdString().size());
    memcpy(pdu->cMsg, Message.toStdString().c_str(), Message.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    updateGroupMsg(pdu);
    free(pdu);pdu = nullptr;
    TcpClient::getInstance()->getTcpSocket()->flush();
}

void Friend::keyPressEvent(QKeyEvent *event)
{
    // 键盘中间的是Qt::Key_Return，小键盘旁边的是Qt::Key_Enter
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        clickedGroupChatBtn();
        return;
    }
    QWidget::keyPressEvent(event);
}

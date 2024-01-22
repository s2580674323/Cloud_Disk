#include "mytcpserver.h"
#include "usedb.h"
#include <QDebug>

MyTcpServer::MyTcpServer()
{

}

MyTcpServer* MyTcpServer::getInstance()
{
    return instance;
}

MyTcpServer* MyTcpServer::instance = new MyTcpServer; // 饿汉模式初始化


void MyTcpServer::DeleteSocket(MyTcpSocket *socket)
{
    for(int i = 0; i < m_TcpSocketList.count(); ++i) {
        if(m_TcpSocketList[i] == socket) {
            m_TcpSocketList[i]->deleteLater(); // 删除socket对象
            m_TcpSocketList.removeAt(i); // 删除链表里指向该对象的指针
            break;
        }
    }
//    for(int i = 0; i < m_TcpSocketList.size(); ++i) {
//        qDebug() << m_TcpSocketList.at(i)->getName();
//    }
}
// 将添加好友的请求转发给在线用户
void MyTcpServer::Transpond(const char *friname, PDU *pdu)
{
    if(friname == nullptr || pdu == nullptr) return;
    QString name(friname);
    for(int i = 0; i < m_TcpSocketList.size(); ++i) {
        if(m_TcpSocketList.at(i)->getName() == name) {
            m_TcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
            break;
        }
    }
}

void MyTcpServer::TranspondALLUser(PDU *pdu, QString SenderName)
{
    if(pdu == nullptr) return;
    for(int i = 0; i < m_TcpSocketList.size(); ++i) {
        QString name = m_TcpSocketList.at(i)->getName();
        if(name == SenderName) continue;
//        int res = useDB::getInstance()->SearchUsr(name.toStdString().c_str());
//        if(res != 1) continue;
        qDebug() << "服务器转发给用户：" << name;
        m_TcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
    }
}

// 当客户端请求可以处理时，自动调用该函数进行处理
void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    MyTcpSocket *pTcpSocket = new MyTcpSocket;
    // 通过 setSocketDescriptor 将刚new的 mytcpsocket 与新连接的套接字描述符关联
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_TcpSocketList.append(pTcpSocket);

    qDebug() << "新连接来了: " << pTcpSocket  << " 目前数量: " << m_TcpSocketList.count();


    void(MyTcpSocket:: *User_Offline_Signal)(MyTcpSocket *socket) = &MyTcpSocket::User_Offline;
    connect(pTcpSocket, User_Offline_Signal, this, &MyTcpServer::DeleteSocket);
}

#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QSettings>
#include <QList>

#include "mytcpsocket.h"

/*
自定义的tcpserver没做什么事情，只设计了一个单例模式
修改ip和port的可以不用ini文件配置
设计一个设置界面也可以修改服务器ip和通信所用的端口
*/

class MyTcpServer : public QTcpServer
{
public:
    MyTcpServer();
    static MyTcpServer* getInstance();

public slots:
    void DeleteSocket(MyTcpSocket *socket);
    void Transpond(const char *friname, PDU *pdu);
    void TranspondALLUser(PDU *pdu, QString SenderName);

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    MyTcpServer(const MyTcpServer&) = delete;
    MyTcpServer& operator=(const MyTcpServer&) = delete;

    static MyTcpServer *instance; // 设计一个单例模式

    QList<MyTcpSocket *> m_TcpSocketList;

};

#endif // MYTCPSERVER_H

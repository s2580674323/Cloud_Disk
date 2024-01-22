#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QTcpSocket>
#include <QDir>
#include <QThreadPool>


#include "protocol.h"
#include "usedb.h"

class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();

    void recv_Msg();
    void client_offline();
    QString getName();
    void ScheduledDataSending();

signals:


private:
    QString m_userName; // 与该socket通信的用户名

    //  文件系统
    QFile *m_file;
    qint64 m_lTotalSize;
    qint64 m_lReceiveSize;
    bool m_bUpLoad = false; // 是否处于上传文件状态

    //
    QString m_strFilePath; // 保存 下载文件时，选中的本地文件的路径
    QTimer *m_pTimer; // 定时发送，减少下载文件时粘包的可能性

signals:
    void User_Offline(MyTcpSocket *socket);
};

#endif // MYTCPSOCKET_H

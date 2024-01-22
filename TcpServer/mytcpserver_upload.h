#ifndef MYTCPSERVER_UPLOAD_H
#define MYTCPSERVER_UPLOAD_H

#include <QObject>
#include <QTcpServer>
#include <QThread>

#include "protocol.h"
#include "thread_upload.h"

class MyTcpServer_UpLoad : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer_UpLoad();

    static MyTcpServer_UpLoad *getInstance();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    static MyTcpServer_UpLoad *self;
};

#endif // MYTCPSERVER_UPLOAD_H

#ifndef DOWNLOADWORK_H
#define DOWNLOADWORK_H

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QFile>
#include <QHostAddress>

class DownLoadWork : public QObject
{
    Q_OBJECT
public:
    explicit DownLoadWork(QObject *parent = nullptr);

    // 连接服务器
    void connectServer(unsigned int port, QString ip);
    void sendDownLoadRequest();
    void recv_Msg();


public slots:
    void setCurPath(QString curpath);
    void setFileName(QString filename);
    void setSaveFilePath(QString savefilepath);

signals:
    void DownLoadThreadConnected();
    void DownLoadThreadFinished();
    void AlreadySentSize(int percent);

private:
    QTcpSocket *m_pTcpSocket;
    QFile *m_file;
    qint64 m_lTotalSize;
    qint64 m_lReceiveSize;
    bool m_bDownLoad = false;

    QString m_CurPath;
    QString m_FileName;
    QString m_SaveFilePath;
};

#endif // DOWNLOADWORK_H

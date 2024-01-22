#ifndef UPLOADTHREAD_H
#define UPLOADTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QHostAddress>


class UpLoadWork : public QObject
{
    Q_OBJECT
public:
    UpLoadWork();

    // 连接服务器
    void connectServer(unsigned int port, QString ip);
    void sendUpLoadNote();
    void UpLoadFile();


public slots:
    void setFilePath(QString filepath);
    void setCurPath(QString curpath);
    void setFileName(QString filename);
    void setFileSize(qint64 filesize);

signals:
    void UpLoadThreadConnected();
    void AlreadySentSize(qint64 size);
    void ReadFileError();
    void UpLoadThreadFinished();

private:
    QTcpSocket *m_pTcpSocket;

    QString m_FilePath;
    QString m_CurPath;
    QString m_FileName;
    qint64 m_FileSize;
    int cnt = 0;

};

#endif // UPLOADTHREAD_H

#include "uploadwork.h"
#include "protocol.h"

UpLoadWork::UpLoadWork()
{

}

void UpLoadWork::connectServer(unsigned int port, QString ip)
{
    qDebug() << "上传文件的子线程:" << QThread::currentThread();
    qDebug() << ip << " " << port;
    m_pTcpSocket = new QTcpSocket;
    m_pTcpSocket->connectToHost(QHostAddress(ip), port);
    // 发
    connect(m_pTcpSocket, &QTcpSocket::connected, this, [=]{
        emit UpLoadThreadConnected();
        qDebug() << "与服务器连接成功";

        qDebug() << "子线程的槽函数:" << QThread::currentThread();
    });
    connect(m_pTcpSocket, &QTcpSocket::disconnected, this, [=]{
        m_pTcpSocket->close();// ?
        m_pTcpSocket->deleteLater();
        emit UpLoadThreadFinished();
    });

    if (m_pTcpSocket->waitForConnected(3000)) {
        sendUpLoadNote();
    } else {
        qDebug() << "连接服务器失败";
    }


    while(1) {
        if(cnt >= 4) {
            qDebug() << m_FilePath << m_FileName << m_FileSize << m_CurPath;
            qDebug() << "资源等待结束   " << cnt;
            break;
        }
    }

    UpLoadFile();
}

void UpLoadWork::sendUpLoadNote()
{
    PDU *pdu = mkPDU(m_CurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memcpy(pdu->cMsg, m_CurPath.toStdString().c_str(), m_CurPath.toStdString().size());
    memcpy(pdu->cData, m_FileName.toStdString().c_str(), m_FileName.toStdString().size());
    memcpy(pdu->cData + STR_MAX_SIZE, &m_FileSize, sizeof(qint64)); // 发送整型要确定好该整型的大小
    m_pTcpSocket->write((char*)pdu, pdu->uiPDULen);
//    m_pTcpSocket->flush();
    free(pdu); pdu = nullptr;
    qDebug() << "发送转换服务器端套接字状态的PDU";
}

void UpLoadWork::UpLoadFile()
{
    QFile file(m_FilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        emit ReadFileError();
        return;
    }

    char *pBuffer = new char[4096](); // 使用 () 进行值初始化，将内存内容初始化为零
    qint64 readSize = 0;// 每次实际读了多少字节
    qint64 sendSize = 0;
    while(1) {
        readSize = file.read(pBuffer, 4096);
        sendSize += readSize;
        emit AlreadySentSize(sendSize * 100 / m_FileSize);
//        qDebug() << sendSize;
        if(readSize > 0 && readSize <= 4096) {
            m_pTcpSocket->write(pBuffer, readSize);
//            m_pTcpSocket->flush();
        }
        else if(readSize == 0) {
            break;
        }
        else {
            emit ReadFileError();
//            QMessageBox::information(nullptr, "error", "上传文件失败:读取文件出错");
            break;
        }
    }
    file.close();
    delete []pBuffer;pBuffer = nullptr;

    qDebug() << "发送的大小： "  << sendSize;
}


void UpLoadWork::setFilePath(QString filepath)
{
    if(filepath.isEmpty()) return;
    m_FilePath = filepath;
    ++cnt;
}

void UpLoadWork::setCurPath(QString curpath)
{
    if(curpath.isEmpty()) return;
    m_CurPath = curpath;
    ++cnt;
}

void UpLoadWork::setFileName(QString filename)
{
    if(filename.isEmpty()) return;
    m_FileName = filename;
    ++cnt;
}

void UpLoadWork::setFileSize(qint64 filesize)
{
    m_FileSize = filesize;
    ++cnt;
}


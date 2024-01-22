#include "downloadwork.h"
#include "protocol.h"


DownLoadWork::DownLoadWork(QObject *parent) : QObject(parent)
{

}

void DownLoadWork::connectServer(unsigned int port, QString ip)
{
    qDebug() << "下载文件的子线程:" << QThread::currentThread();
    qDebug() << ip << " " << port;
    m_pTcpSocket = new QTcpSocket;
    m_file = new QFile;
    m_pTcpSocket->connectToHost(QHostAddress(ip), port);
    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &DownLoadWork::recv_Msg);
    // 发
    connect(m_pTcpSocket, &QTcpSocket::connected, this, [=]{
        emit DownLoadThreadConnected();
        qDebug() << "子线程的槽函数:" << QThread::currentThread();
    });
    connect(m_pTcpSocket, &QTcpSocket::disconnected, this, [=]{
        m_pTcpSocket->close();// ?
        m_pTcpSocket->deleteLater();
        emit DownLoadThreadFinished();
    });



    if (m_pTcpSocket->waitForConnected(3000)) {
        sendDownLoadRequest();
    } else {
        qDebug() << "连接服务器失败";
    }
}

void DownLoadWork::sendDownLoadRequest()
{
    PDU *pdu = mkPDU(m_CurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->cData, m_FileName.toStdString().c_str(), m_FileName.toStdString().size());
    memcpy(pdu->cMsg, m_CurPath.toStdString().c_str(), m_CurPath.toStdString().size());
    m_pTcpSocket->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
}

void DownLoadWork::recv_Msg()
{
    if(m_bDownLoad) { // 接收数据
        QByteArray buffer = m_pTcpSocket->readAll();
        m_file->write(buffer);
        m_lReceiveSize += buffer.size();
        emit AlreadySentSize(m_lReceiveSize * 100 / m_lTotalSize);

//        qDebug() << "接收 : " << buffer.size() << " 当前 : " << m_lReceiveSize;
        if(m_lReceiveSize >= m_lTotalSize) {
            m_file->close();
            m_bDownLoad = false;

            if(m_lReceiveSize == m_lTotalSize) {
//                QMessageBox::information(this, "prompt", "download ok");
            }
            else {
//                QMessageBox::information(this, "prompt", "download end, but not quite right: 文件里似乎多了些东西，但那似乎不影响");
            }
            m_pTcpSocket->disconnectFromHost();
        }
        return;
    }


    uint uiPDULen = 0;
    m_pTcpSocket->read((char*)&uiPDULen, sizeof(uint)); // 先接收sizeof(uint)字节大小，计算消息的总字节
    uint uiMsgLen = uiPDULen - sizeof(PDU);   // 实际消息大小
    PDU *pdu = mkPDU(uiMsgLen);  // 根据实际消息大小申请空间
    m_pTcpSocket->read((char*)pdu + sizeof(int), uiPDULen - sizeof(uint)); //读取剩下的字节

    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:
    {
//        QMessageBox::information(this, "prompt", pdu->cData);
        if(strcmp(pdu->cData, DOWNLOAD_FLLE_START) == 0) {
            qint64 fileSize;
            memcpy(&fileSize, (char*)(pdu->cMsg), sizeof(qint64));
//            qDebug() << "客户端要接收的文件大小 : " << fileSize;
            m_file->setFileName(m_SaveFilePath);
            //以只写的方式打开文件，如果文件不存在，则会自动创建文件
            if(m_file->open(QIODevice::WriteOnly)) {
                m_bDownLoad = true;
                m_lTotalSize = fileSize;
                m_lReceiveSize = 0;
//                ProgressBar::getInstance()->show();
            }
        }
        break;
    }

    }
}

void DownLoadWork::setCurPath(QString curpath)
{
    m_CurPath = curpath;
}

void DownLoadWork::setFileName(QString filename)
{
    m_FileName = filename;
}

void DownLoadWork::setSaveFilePath(QString savefilepath)
{
    m_SaveFilePath = savefilepath;
}

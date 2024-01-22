#include "mytcpsocket.h"
#include <QDebug>
#include <QStringList>
#include "mytcpserver.h"
#include <QFileInfoList>
#include <QTimer>

MyTcpSocket::MyTcpSocket()
{
    connect(this, &MyTcpSocket::readyRead, this, &MyTcpSocket::recv_Msg);
    connect(this, &MyTcpSocket::disconnected, this, &MyTcpSocket::client_offline);

    m_file = new QFile; // 设置文件
    m_pTimer = new QTimer;
    connect(m_pTimer, &QTimer::timeout, this, &MyTcpSocket::ScheduledDataSending);

}

void MyTcpSocket::recv_Msg()
{
    if(m_bUpLoad == 0) { // 接收PDU状态
        qDebug() << "接收PDU来自 socket = " << this;
        uint uiPDULen = 0;
        this->read((char*)&uiPDULen, sizeof(uint)); // 先接收sizeof(uint)字节大小，计算消息的总字节
        uint uiMsgLen = uiPDULen - sizeof(PDU);   // 实际消息大小
        PDU *pdu = mkPDU(uiMsgLen);  // 根据实际消息大小申请空间，创建接收消息的pdu
        this->read((char*)pdu + sizeof(uint), uiPDULen - sizeof(uint)); //读取剩下的字节
//        qDebug() << pdu->uiMsgType << uiPDULen-sizeof(uint);
        PDU *respondPDU = mkPDU(0); // 用于回复的pdu

        switch (pdu->uiMsgType) {
        case ENUM_MSG_TYPE_REGISTER_REQUEST: // 注册请求
        {
            char sName[32], sPwd[32];
            strncpy(sName, pdu->cData, 32);
            strncpy(sPwd, pdu->cData + 32, 32);
            bool ok = useDB::getInstance()->Register(sName, sPwd);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_REGISTER_RESPOND;
            if(ok) {
                strcpy(respondPDU->cData, REGISTER_OK);
                // 注册成功，新建一个该用户的根目录
                QDir dir;
                bool res = dir.mkdir(QString("./User/%1").arg(sName));
                qDebug() << "创建用户目录 : " << res;
            }
            else {
                strcpy(respondPDU->cData, REGISTER_FAILED);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_LOGIN_REQUEST: // 登录请求
        {
            char sName[32], sPwd[32];
            strncpy(sName, pdu->cData, 32);
            strncpy(sPwd, pdu->cData + 32, 32);
            bool ok = useDB::getInstance()->Login(sName, sPwd);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPOND;
            if(ok) {
                strcpy(respondPDU->cData, LOGIN_OK);
                m_userName = sName;
            }
            else {
                strcpy(respondPDU->cData, LOGIN_FAILED);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_ALL_ONLINE_REQUEST: // 查看在线用户, PDU的msg不为空
        {
            QStringList res = useDB::getInstance()->ShowAllOnline();
            uint uiMsgLen = res.size() * 32; //一个名字不超过32字节，中文名不能超过10个字符
            PDU *respdu = mkPDU(uiMsgLen); // 按大小创建PDU
            respdu->uiMsgType = ENUM_MSG_TYPE_ALL_ONLINE_RESPOND;
            for(int i = 0; i < res.size(); ++i) {
                //                                      调用c_str()得到的就是char*的首地址
                memcpy((char *)(respdu->cMsg) + i * 32, res.at(i).toStdString().c_str(),
                       res.at(i).toUtf8().size());// QSring调用size得到的是字符数，不是字节数
            }
            this->write((char*)respdu, respdu->uiPDULen);
            free(respdu);respdu = nullptr;
            break;
        }
        case ENUM_MSG_TYPE_SEARCH_USER_REQUEST: //  搜索用户
        {
            int res = useDB::getInstance()->SearchUsr(pdu->cData);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_SEARCH_USER_RESPOND;
            if(res == -1) {
                strcpy(respondPDU->cData, SEARCH_USR_NO);
            }
            else if(res == 1) {
                strcpy(respondPDU->cData, SEARCH_USR_ONLINE);
            }
            else {
                strcpy(respondPDU->cData, SEARCH_USR_OFFLINE);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:  // 添加好友, 在线用户就进行转发好友请求
        {
            char myName[32], friName[32];
            strncpy(friName, pdu->cData, 32);
            strncpy(myName, pdu->cData + 32, 32);
            int res = useDB::getInstance()->AddFriend(friName, myName);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPOND;
            if(res == 1) {  // 在线
                // 服务器转发来自某个用户添加好友的请求给friName用户
                // pdu就是来自客户端的添加好友请求，消息类型是 request，与上边的respond 不同
                MyTcpServer::getInstance()->Transpond(friName, pdu);
                break;
            }
            else if(res == -1) { // system error
                strcpy(respondPDU->cData, SYSTEM_ERROR);
            }
            else if(res == -2) {  // 已经是好友了
                strcpy(respondPDU->cData, ADD_FRIEND_EXISTED);
            }
            else {  // 不在线
                strcpy(respondPDU->cData, ADD_FRIEND_OFFLINE);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_AGREE: // 同意添加好友，更新数据库，并通知申请者
        {
            char myName[32], friName[32];
            strncpy(friName, pdu->cData, 32);
            strncpy(myName, pdu->cData + 32, 32);
            useDB::getInstance()->UpdateAgreeAddFriend(friName, myName);

            respondPDU->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE: // 拒绝添加好友，并通知申请者
        {
            respondPDU->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST: // 刷新在线好友的列表, Msg不为空
        {
            char myName[32];
            strcpy(myName, pdu->cData);
            QStringList AllFriend = useDB::getInstance()->FlushFriend(myName);
            uint uiMsgLen = AllFriend.size() * 32; //一个名字不超过32字节，中文名不能超过10个字符
            PDU *respdu = mkPDU(uiMsgLen); // 按大小创建PDU
            respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND;
            for(int i = 0; i < AllFriend.size(); ++i) {
                //                                      调用c_str()得到的就是char*的首地址
                memcpy((char *)(respdu->cMsg) + i * 32, AllFriend.at(i).toStdString().c_str(),
                       AllFriend.at(i).toUtf8().size());// QSring调用size得到的是字符数，不是字节数
            }
            this->write((char*)respdu, respdu->uiPDULen);
            free(respdu);respdu = nullptr;
            break;
        }
        case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST: // 删除好友
        {
            char myName[32], friName[32];
            strncpy(friName, pdu->cData, 32);
            strncpy(myName, pdu->cData + 32, 32);
            bool res = useDB::getInstance()->DeleteFriend(friName, myName);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND;
            if(res) {
                strcpy(respondPDU->cData, DELETE_FRIEND_OK);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST: // 请求私聊
        {
            char friName[32];
            strncpy(friName, pdu->cData, 32);
            MyTcpServer::getInstance()->Transpond(friName, pdu);

            break;
        }
        case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST: // 转发用户的群聊消息
        {
            QString SenderName = QString(pdu->cData);
            MyTcpServer::getInstance()->TranspondALLUser(pdu, SenderName);
            break;
        }
        case ENUM_MSG_TYPE_CREATE_DIR_REQUEST: // 创建文件夹
        {
            // cData: userName  newDirName;    cMsg:CurPath
            QDir dir;
            QString CurPath = QString((char*)pdu->cMsg);
            bool res = dir.exists(CurPath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_RESPOND;
            if(res) { // 用户当前所在的目录存在
                char newDirName[32] = {};
                memcpy(newDirName, pdu->cData + 32, 32);
                QString nextPath = QString("%1/%2").arg(CurPath).arg(newDirName);
                res = dir.exists(nextPath);
                if(res) { // 要创建的文件夹已存在
                    strcpy(respondPDU->cData, NEW_DIR_EXISTED);
                }
                else {
                    dir.mkdir(nextPath);
                    strcpy(respondPDU->cData, CREATE_NEW_DIR_OK);
                }
            }
            else {
                strcpy(respondPDU->cData, USER_CURRENT_DIR_NOT_EXIST);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_REQUEST:  // 刷新当前目录的文件
        {
            QString strCurPath = QString((char*)pdu->cMsg);
    //        qDebug() << strCurPath;
            QDir dir(strCurPath);
            QFileInfoList fileInfoList = dir.entryInfoList();// 返回一个当前目录下的file信息
            int fileCount = fileInfoList.size();
            PDU *respdu = mkPDU(sizeof(FileInfo) * fileCount);
            respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_RESPOND;
            // FileInfo是自定义的，回复给客户端的结构体，只包含file的名字和类型
            FileInfo *tmp_fileinfo = nullptr;// 每次指向一块大小为sizeof(FileInfo)空间的首地址
            for(int i = 0; i < fileInfoList.size(); ++i) {
                tmp_fileinfo = (FileInfo*)respdu->cMsg + i; // +i每次跳过sizeof(FileInfo)大小
                QString tmp_fileName = fileInfoList.at(i).fileName();
                memcpy(tmp_fileinfo->cName, tmp_fileName.toStdString().c_str(), tmp_fileName.toStdString().size());
                if(fileInfoList.at(i).isDir()) { // 文件夹标志为0
                    tmp_fileinfo->iFileType = 0;
                }
                else {
                    tmp_fileinfo->iFileType = 1; // 文件标志为1
                }
    //            qDebug() << tmp_fileName << fileInfoList.at(i).size() / 1024 << " KB";
            }
            this->write((char*)respdu, respdu->uiPDULen);
            free(respdu); respdu = nullptr;
            qDebug() << "发送当前目录信息";
            break;
        }
        case ENUM_MSG_TYPE_DELETE_DIR_REQUEST: // 删除文件夹
        {
            QString dirName = QString(pdu->cData);
            QString curPath = QString((char*)pdu->cMsg);
            QString filePath = QString("%1/%2").arg(curPath).arg(dirName);
            QFileInfo fileInfo(filePath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_RESPOND;
//            qDebug() << filePath;
            if(fileInfo.isDir()) {
                QDir dir;
                dir.setPath(filePath); // 因为删除文件夹的函数没有参数，所以要先设置上该文件夹的路径
                bool res = dir.removeRecursively(); // Removes the directory, including all its contents.
                if(res) {
                    strcpy(respondPDU->cData, DELETE_DIR_OK);
                }
                else {
                    strcpy(respondPDU->cData, DELETE_DIR_FAILED);
                }
            }
            else {
                strcpy(respondPDU->cData, DELETE_DIR_ERROR);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            qDebug() << "删除文件结束, 向服务器回复";
            break;
        }
        case ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_REQUEST:  // 重命名
        {// fileName  newName
            char fileName[32] = {}, newName[32] = {};
            memcpy(fileName, pdu->cData, 32);
            memcpy(newName, pdu->cData + 32, 32);
            QString curPath = QString((char*)pdu->cMsg);
    //        QFileInfo fileInfo(filePath);
            QDir dir;
            QString oldPath = QString("%1/%2").arg(curPath).arg(fileName);
            QString newPath = QString("%1/%2").arg(curPath).arg(newName);
//            qDebug() << oldPath << newPath;
            bool res = dir.rename(oldPath, newPath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_RESPOND;
            if(res) {
                strcpy(respondPDU->cData, RENAME_DIR_OK);
            }
            else {
                strcpy(respondPDU->cData, RENAME_DIR_FAILED);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_ENTER_DIR_REQUEST: // 进入文件夹
        {
            QString curPath = QString((char*)pdu->cMsg);
            QString dirName = QString(pdu->cData);
            QString nextDirPath = QString("%1/%2").arg(curPath).arg(dirName);
            QFileInfo fileInfo(nextDirPath);// 用QFileInfo判断该路径是否为文件夹
    //        qDebug() << nextDirPath;
            if(fileInfo.isDir()) { // 如果nextDirPath为文件夹，就返回给客户端该目录下的文件信息(这部分就是刷新文件的代码)
                QDir dir(nextDirPath);
                QFileInfoList fileInfoList = dir.entryInfoList();// 返回一个当前目录下的file信息
                int fileCount = fileInfoList.size();
                PDU *respdu = mkPDU(sizeof(FileInfo) * fileCount);
                respdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_RESPOND;
                strcpy(respdu->cData, ENTER_DIR_OK);
                // FileInfo是自定义的，回复给客户端的结构体，只包含file的名字和类型
                FileInfo *tmp_fileinfo = nullptr;// 每次指向一块大小为sizeof(FileInfo)空间的首地址
                for(int i = 0; i < fileInfoList.size(); ++i) {
                    tmp_fileinfo = (FileInfo*)respdu->cMsg + i; // +i每次跳过sizeof(FileInfo)大小
                    QString tmp_fileName = fileInfoList.at(i).fileName();
                    memcpy(tmp_fileinfo->cName, tmp_fileName.toStdString().c_str(), tmp_fileName.toStdString().size());
                    if(fileInfoList.at(i).isDir()) { // 文件夹标志为0
                        tmp_fileinfo->iFileType = 0;
                    }
                    else {
                        tmp_fileinfo->iFileType = 1; // 文件标志为1
                    }
                }
                this->write((char*)respdu, respdu->uiPDULen);
                free(respdu); respdu = nullptr;
            }
            else {
                respondPDU->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_RESPOND;
                strcpy(respondPDU->cData, ENTER_DIR_FAILED);
            }
            qDebug() << "返回双击该项目的回复";
            break;
        }
        case ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST: // 接收上传文件请求，修改服务器为接收文件数据的状态
        {
            QString curPath = QString((char*)pdu->cMsg);
            char fileName[STR_MAX_SIZE];
            qint64 fileSize;
            memcpy(fileName, pdu->cData, STR_MAX_SIZE);
            memcpy(&fileSize, pdu->cData + STR_MAX_SIZE, sizeof(qint64));
            qDebug() << curPath << fileName << fileSize;

            QString strPath = QString("%1/%2").arg(curPath).arg(fileName);
            m_file->setFileName(strPath);
            // 以只写的方式打开文件，如果文件不存在，则会自动创建文件
            if(m_file->open(QIODevice::WriteOnly)) {
                m_bUpLoad = true;
                m_lTotalSize = fileSize;
                m_lReceiveSize = 0;
                qDebug() << "该套接字转换为接收文件状态";
            }
            break;
        }
        case ENUM_MSG_TYPE_DELETE_FILE_REQUEST: // 删除文件
        {
            QString fileName = QString(pdu->cData);
            QString curPath = QString((char*)pdu->cMsg);
            QString filePath = QString("%1/%2").arg(curPath).arg(fileName);
            QFileInfo fileInfo(filePath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_RESPOND;
            if(fileInfo.isFile()) {
                QDir dir;
                bool res = dir.remove(filePath);
                if(res) {
                    strcpy(respondPDU->cData, DELETE_FILE_OK);
                }
                else {
                    strcpy(respondPDU->cData, DELETE_FILE_FAILED);
                }
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        case ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST: // 接收下载文件的请求
        {
            QString fileName = QString(pdu->cData);
            QString curPath = QString((char*)pdu->cMsg);
            QString filePath = QString("%1/%2").arg(curPath).arg(fileName);
            qDebug() << filePath;
            // 把服务器curPath中的fileName文件发送给客户端
            respondPDU->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
            m_file->setFileName(filePath);
            if(!m_file->open(QIODevice::ReadOnly)) {
                strcpy(respondPDU->cData, DOWNLOAD_FILE_OPEN_FILE_FAILED);
                this->write((char*)respondPDU, respondPDU->uiPDULen);
            }
            else {
                PDU *respdu = mkPDU(sizeof(qint64)); // 要把文件大小先发给客户端
                respdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND;
                strcpy(respdu->cData, DOWNLOAD_FLLE_START);
                qint64 fileSize = m_file->size();
                memcpy((char*)respdu->cMsg, &fileSize, sizeof(qint64));
                this->write((char*)respdu, respdu->uiPDULen);

                qDebug() << filePath << fileName << fileSize;

                m_pTimer->start(1000); // 延迟1s启动传输程序
            }
            break;
        }
        case ENUM_MSG_TYPE_SHARE_FILE_REQUEST:  // 转发分享文件的请求
        {
            char receiverName[32] = {}, senderName[32];
            memcpy(receiverName, pdu->cData, 32);
            memcpy(senderName, pdu->cData + 32, 32);
            QString shareFilePath = QString((char*)pdu->cMsg);
//            qDebug() << receiverName << senderName << shareFilePath;
            QFile file(shareFilePath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
            if(file.exists()) {
                MyTcpServer::getInstance()->Transpond(receiverName, pdu);
                strcpy(respondPDU->cData, SHARE_FILE_OK);
                this->write((char*)respondPDU, respondPDU->uiPDULen);
            }
            else {
                strcpy(respondPDU->cData, SHARE_FILE_PARH_ERROR);
                this->write((char*)respondPDU, respondPDU->uiPDULen);
            }
            break;
        }
        case ENUM_MSG_TYPE_SHARE_FILE_AGREE:  // 转发分享文件的回复:好友同意接收
        {
            char senderName[32];
            memcpy(senderName, pdu->cData + 32, 32);
            // 修改pdu为回复状态，且cData存回复内容
            pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
            memset(pdu->cData, 0, sizeof(pdu->cData));
            strcpy(pdu->cData, SHARE_FILE_AGREE);
            MyTcpServer::getInstance()->Transpond(senderName, pdu); // 把好友接收情况发给sender
            break;
        }
        case ENUM_MSG_TYPE_SHARE_FILE_REFUSE:  // 转发分享文件的回复:好友拒绝接收
        {
            char senderName[32];
            memcpy(senderName, pdu->cData + 32, 32);
            // 修改pdu为回复状态，且cData存回复内容
            pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_RESPOND;
            memset(pdu->cData, 0, sizeof(pdu->cData));
            strcpy(pdu->cData, SHARE_FILE_REFUSE);
            MyTcpServer::getInstance()->Transpond(senderName, pdu); // 把好友接收情况发给sender
            break;
        }
        case ENUM_MSG_TYPE_MOVE_FILE_REQUEST:  // 移动文件
        {
            // cData(char): filePathLen(int), destPathLen(int), m_strMoveFileName
            // cMsg(int): m_strMoveFilePath, m_strMoveDestDirPath
            int filePathLen, destPathLen;
            memcpy(&filePathLen, pdu->cData, sizeof(int));
            mempcpy(&destPathLen, pdu->cData + sizeof(int), sizeof(int));
            QString moveFileName = QString(pdu->cData + 2 * sizeof(int));
            char *strMoveFilePath = new char[filePathLen + 1](); // 在 () 内传递空参数将内存初始化为零
            char *strMoveDestDirPath = new char[destPathLen + 1]();// 在 () 内传递空参数将内存初始化为零
            memcpy(strMoveFilePath, (char*)(pdu->cMsg), filePathLen);
            memcpy(strMoveDestDirPath, (char*)(pdu->cMsg) + filePathLen + 1, destPathLen);
            qDebug() << moveFileName.toStdString().size() << moveFileName << "\n"
                     << filePathLen << strMoveFilePath << "\n"
                     << destPathLen << strMoveDestDirPath << "\n\n";
            // 目标：把文件strMoveFilePath移动到strMoveDestDirPath目录下，该文件名为moveFileName
            QFileInfo fileInfo(strMoveDestDirPath);
            respondPDU->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPOND;
            if(fileInfo.isDir()) {
                QString newPath = QString("%1/%2").arg(strMoveDestDirPath).arg(moveFileName);
                bool res = QFile::rename(strMoveFilePath, newPath);

                if(res) {
                    strcpy(respondPDU->cData, MOVE_FILE_OK);
                }
                else {
                    strcpy(respondPDU->cData, MOVE_FILE_FIALED);
                }
            }
            else {
                strcpy(respondPDU->cData, MOVE_FILE_ERROR);
            }
            this->write((char*)respondPDU, respondPDU->uiPDULen);

            delete []strMoveFilePath;strMoveFilePath = nullptr;
            delete []strMoveDestDirPath;strMoveDestDirPath = nullptr;
            break;
        }
        case ENUM_MSG_TYPE_TEST_DELAY_REQUEST:
        {
            respondPDU->uiMsgType = ENUM_MSG_TYPE_TEST_DELAY_RESPOND;
            this->write((char*)respondPDU, respondPDU->uiPDULen);
            break;
        }
        default:
            break;
    }

        free(pdu);pdu = nullptr;
        free(respondPDU);respondPDU = nullptr;
    }
    else { // 接收上传的文件状态，接收文件的二进制流
        qDebug() << "接收文件的二进制流，来自 socket = " << this;
        QByteArray buffer = readAll();
        m_file->write(buffer);
        m_lReceiveSize += buffer.size();
        if(m_lReceiveSize >= m_lTotalSize) {
            m_file->close();
            m_bUpLoad = false;

//            PDU *respondPDU = mkPDU(0);
//            respondPDU->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND;
//            if(m_lReceiveSize == m_lTotalSize) strcpy(respondPDU->cData, UPLOAD_FILE_OK);
//            else {
//                strcpy(respondPDU->cData, UPLOAD_FILE_FAILED);
//            }
//            this->write((char*)respondPDU, respondPDU->uiPDULen);
//            free(respondPDU);respondPDU = nullptr;

            qDebug() << "接收数据完毕，total = " << m_lTotalSize << " recvSize = " << m_lReceiveSize;

            this->disconnectFromHost();
        }
    }

}

void MyTcpSocket::client_offline()
{
    // 客户端的socket断开，disconnected信号触发
    // 先把在线状态置为0，触发信号给mytcpserver，然后去mytcpserver里的m_tcpsocket链表里删除对应的mytcpsocket
    if(!m_userName.isEmpty()) {
        useDB::getInstance()->Offline(m_userName.toStdString().c_str());
    }
    qDebug() << "断开连接 : " << this;
    emit User_Offline(this);
}

QString MyTcpSocket::getName()
{
    return m_userName;
}

void MyTcpSocket::ScheduledDataSending()
{
    m_pTimer->stop();

    char *pBuffer = new char[4096];
    qint64 readSize = 0;// 每次实际读了多少字节
    while(1) {
        readSize = m_file->read(pBuffer, 4096); // m_file已经在接收请求时打开了
//        qDebug() << "发送 : " << readSize;
        if(readSize > 0 && readSize <= 4096) {
            this->write(pBuffer, readSize);
        }
        else if(readSize == 0) {
            break;
        }
        else {
            break;
        }
    }
    m_file->close();
    delete []pBuffer;pBuffer = nullptr;
}

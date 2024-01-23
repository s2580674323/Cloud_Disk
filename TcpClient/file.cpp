#include "file.h"
#include "tcpclient.h"



File::File(QWidget *parent) : QWidget(parent)
{
    m_pTimer = new QTimer; // 定时

    QFont font("Agency FB", 14);

    m_pFileListW = new QListWidget;
    m_pFileListW->setFont(font);

    m_pShowFileName = new QLabel("文件名");
    m_pShowFileName->setFont(font);

    m_pShowDirPath = new QLabel("当前路径: ");
    m_pDirPath = new QLabel; // 展示路径
    m_pShowDirPath->setFont(font);
    m_pDirPath->setFont(font);
    m_pShowDirPath->setFixedWidth(120);

    // 最顶端  展示路径
    QHBoxLayout *pDirPath = new QHBoxLayout;
    pDirPath->addWidget(m_pShowDirPath);
    pDirPath->addWidget(m_pDirPath);

    QVBoxLayout *pVFileListLayout = new QVBoxLayout;
    pVFileListLayout->addWidget(m_pShowFileName);
    pVFileListLayout->addWidget(m_pFileListW);

    QFont Btnfont("Agency FB", 12);

    m_pBackBtn = new QPushButton("返回上一级");
    m_pCreateDirBtn = new QPushButton("创建文件夹");
    m_pDeleteDirBtn = new QPushButton("删除文件夹");
    m_pReNameDirBtn = new QPushButton("重命名");
    m_pFlushDirBtn = new QPushButton("刷新当前目录");

    m_pBackBtn->setFont(Btnfont);
    m_pCreateDirBtn->setFont(Btnfont);
    m_pDeleteDirBtn->setFont(Btnfont);
    m_pReNameDirBtn->setFont(Btnfont);
    m_pFlushDirBtn->setFont(Btnfont);

    QVBoxLayout *pVDirLayout = new QVBoxLayout;
    pVDirLayout->addWidget(m_pBackBtn);
    pVDirLayout->addWidget(m_pFlushDirBtn);
    pVDirLayout->addWidget(m_pCreateDirBtn);
    pVDirLayout->addWidget(m_pDeleteDirBtn);
    pVDirLayout->addWidget(m_pReNameDirBtn);


    m_pUpLoadFileBtn = new QPushButton("上传文件");
    m_pDeleteFileBtn = new QPushButton("删除文件");
    m_pDownLoadFileBtn = new QPushButton("下载文件");
    m_pShareFileBtn = new QPushButton("分享文件");
    m_pMoveFileBtn = new QPushButton("移动文件");
    m_pMoveFileConfirmBtn = new QPushButton("确定(选好文件夹点击)");
    m_pMoveFileConfirmBtn->setEnabled(false);

    m_pUpLoadFileBtn->setFont(Btnfont);
    m_pDeleteFileBtn->setFont(Btnfont);
    m_pDownLoadFileBtn->setFont(Btnfont);
    m_pShareFileBtn->setFont(Btnfont);
    m_pMoveFileBtn->setFont(Btnfont);
    m_pMoveFileConfirmBtn->setFont(Btnfont);

    QVBoxLayout *pVFileLayout = new QVBoxLayout;
    pVFileLayout->addWidget(m_pUpLoadFileBtn);
    pVFileLayout->addWidget(m_pDeleteFileBtn);
    pVFileLayout->addWidget(m_pDownLoadFileBtn);
    pVFileLayout->addWidget(m_pShareFileBtn);
    pVFileLayout->addWidget(m_pMoveFileBtn);
    pVFileLayout->addWidget(m_pMoveFileConfirmBtn);

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addLayout(pVFileListLayout);
    pMain->addLayout(pVDirLayout);
    pMain->addLayout(pVFileLayout);

    QVBoxLayout *pFinal = new QVBoxLayout;
    pFinal->addLayout(pDirPath);
    pFinal->addLayout(pMain);

    setLayout(pFinal);

    // 利用flushScreenSignal信号，刷新当前目录
    // 加上这步，数据好像会
//    connect(TcpClient::getInstance(), &TcpClient::flushScreenSignal, this, &File::clickedFlushScreenBtn);

    connect(m_pCreateDirBtn, &QPushButton::clicked, this, &File::clickedCreateDirBtn);
    connect(m_pFlushDirBtn, &QPushButton::clicked, this, &File::clickedFlushScreenBtn);
    connect(m_pDeleteDirBtn, &QPushButton::clicked, this, &File::clickedDeleteDirBtn);
    connect(m_pReNameDirBtn, &QPushButton::clicked, this, &File::clickedRenameDirBtn);
    connect(m_pBackBtn, &QPushButton::clicked, this, &File::clickedBackBtn);

    connect(m_pFileListW, &QListWidget::doubleClicked, this, &File::doubleClickedItem);

    connect(m_pUpLoadFileBtn, &QPushButton::clicked, this, &File::clickedUpLoadFileBtn);
    connect(m_pDeleteFileBtn, &QPushButton::clicked, this, &File::clickedDeleteFileBtn);
    connect(m_pDownLoadFileBtn, &QPushButton::clicked, this, &File::clickedDownLoadFileBtn);
    connect(m_pShareFileBtn, &QPushButton::clicked, this, &File::clickedShareFileBtn);
    connect(m_pMoveFileBtn, &QPushButton::clicked, this, &File::clickedMoveFileBtn);
    connect(m_pMoveFileConfirmBtn, &QPushButton::clicked, this, &File::clickedMoveFileConfirmBtn);

    connect(m_pTimer, &QTimer::timeout, this, &File::ScheduledDataSending);
}

void File::updateFileList(const PDU *pdu)
{
    if(pdu == nullptr) return;
    // 刷新前先清空之前的元素
    for(int i = 0; i < m_pFileListW->count(); ++i) {
        QListWidgetItem *item = m_pFileListW->item(i); // 指向第i个元素
        delete item; item = nullptr;
        // 因为QListWidgetItem是new出来的，所以这里delete掉，但是PDU是malloc出来的
    }
    m_pFileListW->clear(); // 不能简单的clear，需要先free掉之前申请的空间

    FileInfo *pFileInfo = nullptr;
    int fileCount = pdu->uiMsgLen / sizeof(FileInfo);
    for(int i = 0; i < fileCount; ++i) { // 每一个元素是QListWidgetItem
        pFileInfo = (FileInfo *)(pdu->cMsg) + i;
        if(strcmp(pFileInfo->cName, ".") == 0 || strcmp(pFileInfo->cName, "..") == 0) continue;
        // 同时使用 .. 和 返回按钮，不好进行同步路径，所以这里只采用了返回按钮
//        qDebug() << pFileInfo->cName << pFileInfo->iFileType;
        QListWidgetItem *pItem = new QListWidgetItem;
        if(pFileInfo->iFileType == 0) {
            pItem->setIcon(QIcon(":/res/directory.png"));
            pItem->setData(Qt::UserRole, 0); // 设置标志0，文件夹
        }
        else if(pFileInfo->iFileType == 1) {
            pItem->setIcon(QIcon(":/res/file.png"));
            pItem->setData(Qt::UserRole, 1); // 普通文件
        }
        pItem->setText(pFileInfo->cName);
        m_pFileListW->addItem(pItem);
    }
}

QString File::getNextDirectory()
{
    return m_NextDirectory;
}

void File::updateDirectoryPath(QString dirpath)
{
    int pos = dirpath.indexOf("/", 3);
    dirpath = dirpath.mid(pos); // 去掉User文件夹，从用户名文件夹开始显示
    m_pDirPath->setText(dirpath);
}

QString File::getSaveFilePath()
{
    return m_strSaveFilePath;
}

QString File::getShareFileName()
{
    return m_strShareFileName;
}

void File::updateSaveFilePath(QString SaveFilePath)
{
    m_strSaveFilePath = SaveFilePath;
}

void File::DownLoadFile(QString strCurPath, QString fileName, QString strSaveFilePath)
{
/*
子线程首先发送:用PDU封装好的要下载的文件路径给服务器端
服务器端解析PDU后，判断该路径是否为文件路径，如果是文件，就将文件大小发送给子线程的socket
子线程解析PDU得到文件大小，然后进入接收二进制流的状态
*/
    QThread *pthread = new QThread;
    DownLoadWork *pDownLoadWork = new DownLoadWork;
    pDownLoadWork->moveToThread(pthread);

    pthread->start();
    // 发
    connect(this, &File::SendCurPath_DownLoad, pDownLoadWork, &DownLoadWork::setCurPath);
    connect(this, &File::SendFileName_DownLoad, pDownLoadWork, &DownLoadWork::setFileName);
    connect(this, &File::SendSaveFilePath_DownLoad, pDownLoadWork, &DownLoadWork::setSaveFilePath);
    connect(this, &File::StartConnect_DownLoad, pDownLoadWork, &DownLoadWork::connectServer);

    //收
    connect(pDownLoadWork, &DownLoadWork::AlreadySentSize, this, [=](int percent){
        ProgressBar::getInstance()->updateProgressUI(percent);
    });
    connect(pDownLoadWork, &DownLoadWork::DownLoadThreadConnected, this, [=]{
        QMessageBox::information(nullptr, "prompt", "下载文件子线程连接服务器成功");
    });
    connect(pDownLoadWork, &DownLoadWork::DownLoadThreadFinished, this, [=]{
        pthread->quit();
        pthread->wait();
        pDownLoadWork->deleteLater();
        pthread->deleteLater();
        qDebug() << "下载文件完毕，delete掉下载文件子线程和工作类";
    });

    emit SendCurPath_DownLoad(strCurPath);
    emit SendFileName_DownLoad(fileName);
    emit SendSaveFilePath_DownLoad(strSaveFilePath);

    QThread::sleep(1);
    emit StartConnect_DownLoad(TcpClient::getInstance()->getPort(), TcpClient::getInstance()->getIP());
    ProgressBar::getInstance()->show();
}

void File::clickedCreateDirBtn()
{// 新的文件夹名，用户名，当前目录
    QString newDirName = QInputDialog::getText(this, "新建文件夹", "新文件夹名字(中文不能超过40个字符)");
    if(newDirName.isEmpty()) return;
    if(newDirName.toStdString().size() > STR_MAX_SIZE - 1) {
        QMessageBox::information(this, "新建文件夹失败", "请按要求输入新建的文件夹名");
        return;
    }
    QString userName = TcpClient::getInstance()->getMyUsrName();
    QString curPath = TcpClient::getInstance()->getCurrentFilePath();
    PDU *pdu = mkPDU(curPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
    memcpy(pdu->cData, userName.toStdString().c_str(), userName.toStdString().size());
    memcpy(pdu->cData + STR_MAX_SIZE, newDirName.toStdString().c_str(), newDirName.toStdString().size());
    memcpy(pdu->cMsg, curPath.toStdString().c_str(), curPath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu);pdu = nullptr;
}



void File::clickedDeleteDirBtn()
{
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    if(strCurPath.isEmpty()) return;
    if(m_pFileListW->currentItem() == nullptr) return;
    QListWidgetItem *item = m_pFileListW->currentItem();
    QString dirName = item->text();
    int type = item->data(Qt::UserRole).toInt();
    if(type != 0) {
        QMessageBox::information(this, "prompt", "该按钮只能删除文件夹");
        return;
    }
    //    qDebug() << strCurPath << fileName;
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "question", "你确定要删除该文件夹吗?(无法恢复)",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply != QMessageBox::Yes) return;

    PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_REQUEST;
    memcpy(pdu->cData, dirName.toStdString().c_str(), dirName.toStdString().size());
    memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
}

void File::clickedRenameDirBtn()
{
    if(m_pFileListW->currentItem() == nullptr) return;
    QString newName = QInputDialog::getText(this, "重命名", "请输入新名字\n"
                                                         "注意事项: \n"
                                                         "1.中文不能超过40个字符\n"
                                                         "2.修改名字时请添加上文件原后缀\n");
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    if(newName.isEmpty() || strCurPath.isEmpty()) return;
//    if(newName.contains(".")) {
//        QMessageBox::information(this, "prompt", "名字中不能包含特殊字符");
//        return;
//    }
    QListWidgetItem *item = m_pFileListW->currentItem();
    QString fileName = item->text();

    PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_REQUEST;
    memcpy(pdu->cData, fileName.toStdString().c_str(), fileName.toStdString().size());
    memcpy(pdu->cData + STR_MAX_SIZE, newName.toStdString().c_str(), newName.toStdString().size());
    memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
}

void File::clickedBackBtn()
{
    QStack <QString> &m_DirPathStack = TcpClient::getInstance()->getFilePathStack();
    if(m_DirPathStack.isEmpty()) return;
    QString preDirectory = m_DirPathStack.top(); // 得到上一级的目录路径
    m_DirPathStack.pop();
    TcpClient::getInstance()->updateCurrentFilePath(preDirectory); // 把目录修改为上一级目录
    updateDirectoryPath(preDirectory);
    clickedFlushScreenBtn(); // 该函数就是向服务器请求当前目录的信息
}

void File::clickedFlushScreenBtn()
{
    QString strCurrentPath = TcpClient::getInstance()->getCurrentFilePath();
    updateDirectoryPath(strCurrentPath);
    PDU *pdu = mkPDU(strCurrentPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_REQUEST;
    memcpy(pdu->cMsg, strCurrentPath.toStdString().c_str(), strCurrentPath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
    qDebug() << "向服务器请求更新目录";
}

void File::doubleClickedItem() //  双击item进行请求
{
    if(m_pFileListW->currentItem() == nullptr) return;
    QListWidgetItem *item = m_pFileListW->currentItem();
    int type = item->data(Qt::UserRole).toInt();
    //    qDebug() << item->text() << type;
    if(type == 0) { // 判断是否为文件夹
        QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
        QString dirName = item->text();
        QString nextDirPath = QString("%1/%2").arg(strCurPath).arg(dirName);
        m_NextDirectory = nextDirPath; // 不管有没有进入成功，先保存一下要进入的文件夹的路径
        PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
        pdu->uiMsgType = ENUM_MSG_TYPE_ENTER_DIR_REQUEST;
        memcpy(pdu->cData, dirName.toStdString().c_str(), dirName.toStdString().size());
        memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
        TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
        free(pdu);pdu = nullptr;
        qDebug() << "向服务器请求进入下一级目录";
    }
}

void File::clickedUpLoadFileBtn() // 发送上传文件请求
{
    // 获取要上传到云盘的路径
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    m_strFilePath = QFileDialog::getOpenFileName(); // 存一下选中的文件的路径
    if(strCurPath.isEmpty() || m_strFilePath.isEmpty()) return;
    int pos = m_strFilePath.lastIndexOf("/");
    // 获得文件名
    QString fileName = m_strFilePath.mid(pos + 1);//返回从pos+1往后的所有字符,  right(n)，返回后n个字符
//    qDebug() << m_strFilePath << fileName;
    if(fileName.toStdString().size() > STR_MAX_SIZE - 1) {
        QMessageBox::information(this, "prompt", "请缩减文件名长度再进行上传(中文不能超过40个字符)");
        return;
    }

    // 获得文件大小
    QFile file(m_strFilePath);
    qint64 fileSize = file.size();
    file.close();


//    PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
//    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
//    memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
//    memcpy(pdu->cData, fileName.toStdString().c_str(), fileName.toStdString().size());
//    memcpy(pdu->cData + STR_MAX_SIZE, &fileSize, sizeof(qint64)); // 发送整型要确定好该整型的大小
//    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
//    free(pdu); pdu = nullptr;

    ProgressBar::getInstance()->show();

    qDebug() << "文件大小 : " << fileSize;

/*
在子线程中新建socket与服务器建立连接，然后发送请求上传文件的PDU
服务器在解析完PDU后进入接收二进制流的状态
子线程中发送完PDU之后，只发送了文件的完整数据
服务器端接收完所有的文件数据后，断开连接
这时子线程的socket也发出断开连接的信号，触发自定义信号通知主线程进行释放内存
*/
    QThread *pthread = new QThread;
    UpLoadWork *pUpLoadWork = new UpLoadWork;
    pUpLoadWork->moveToThread(pthread);

    pthread->start();

    // 发
    connect(this, &File::SendFilePath_UpLoad, pUpLoadWork, &UpLoadWork::setFilePath);
    connect(this, &File::SendCurPath_UpLoad, pUpLoadWork, &UpLoadWork::setCurPath);
    connect(this, &File::SendFileName_UpLoad, pUpLoadWork, &UpLoadWork::setFileName);
    connect(this, &File::SendFileSize_UpLoad, pUpLoadWork, &UpLoadWork::setFileSize);
    connect(this, &File::StartConnect_UpLoad, pUpLoadWork, &UpLoadWork::connectServer);
    // 收
    // 这个槽函数进行连接的时候一定要加this，不然这个QMessageBox就跑子线程中去了
    // 报错：QBasicTimer::stop: Failed. Possibly trying to stop from a different thread
    connect(pUpLoadWork, &UpLoadWork::UpLoadThreadConnected, this, [=]{
        QMessageBox::information(nullptr, "prompt", "上传文件子线程连接服务器成功");
    });
    connect(pUpLoadWork, &UpLoadWork::AlreadySentSize, ProgressBar::getInstance(), &ProgressBar::updateProgressUI);
    connect(pUpLoadWork, &UpLoadWork::ReadFileError, this, [=]{
        QMessageBox::information(nullptr, "prompt", "上传文件子线程:读取本地文件出错");
    });
    // 这个槽函数进行连接的时候一定要加this，不然这个对线程的wait是在子线程中
    // 报错：QThread::wait: Thread tried to wait on itself
    connect(pUpLoadWork, &UpLoadWork::UpLoadThreadFinished, this, [=]{
        pthread->quit();
        pthread->wait();
        pUpLoadWork->deleteLater();
        pthread->deleteLater();
        qDebug() << "上传文件完毕，delete掉上传文件子线程和工作类";
    });

    emit SendFilePath_UpLoad(m_strFilePath);
    emit SendCurPath_UpLoad(strCurPath);
    emit SendFileName_UpLoad(fileName);
    emit SendFileSize_UpLoad(fileSize);

    QThread::sleep(1);
    emit StartConnect_UpLoad(TcpClient::getInstance()->getPort(), TcpClient::getInstance()->getIP());

    // 等待线程退出
    // 子线程结束自动释放内存
//    connect(pthread, &UpLoadWork::UpLoadThreadFinished, [&pthread]{
//        pthread->wait();
//        pthread->deleteLater();
//        pthread = nullptr;
//        qDebug() << "释放子线程内存";
//    });


//    m_pTimer->start(1000); // 延迟1s进行上传文件的操作
}

void File::ScheduledDataSending()  //
{
//    m_pTimer->stop();

//    QFile file(m_strFilePath);
//    if(!file.open(QIODevice::ReadOnly)) {
//        QMessageBox::information(this, "prompt", "打开文件失败");
//        return;
//    }
//    char *pBuffer = new char[4096](); // 使用 () 进行值初始化，将内存内容初始化为零
//    qint64 readSize = 0;// 每次实际读了多少字节
//    qint64 sendSize = 0;
//    while(1) {
//        readSize = file.read(pBuffer, 4096);
//        sendSize += readSize;
//        ProgressBar::getInstance()->updateProgressUI(sendSize);
//        if(readSize > 0 && readSize <= 4096) {
//            TcpClient::getInstance()->getTcpSocket()->write(pBuffer, readSize);
//        }
//        else if(readSize == 0) {
//            break;
//        }
//        else {
//            QMessageBox::information(this, "error", "上传文件失败:读取文件出错");
//            break;
//        }
//    }
//    file.close();
//    delete []pBuffer;pBuffer = nullptr;
}

void File::clickedDeleteFileBtn()
{
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    if(strCurPath.isEmpty()) return;
    if(m_pFileListW->currentItem() == nullptr) return;
    QListWidgetItem *item = m_pFileListW->currentItem();
    QString fileName = item->text();
    int type = item->data(Qt::UserRole).toInt();
    if(type != 1) {
        QMessageBox::information(this, "prompt", "该按钮只能删除文件");
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "question", "你确定要删除该文件吗?(无法恢复)",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply != QMessageBox::Yes) return;

    PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
    memcpy(pdu->cData, fileName.toStdString().c_str(), fileName.toStdString().size());
    memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
    qDebug() << "向服务器请求删除文件";
}

void File::clickedDownLoadFileBtn()
{
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    if(strCurPath.isEmpty()) return;
    if(m_pFileListW->currentItem() == nullptr) return;
    QListWidgetItem *item = m_pFileListW->currentItem();
    int type = item->data(Qt::UserRole).toInt();
    if(type != 1) {
        QMessageBox::information(this, "prompt", "目前仅支持下载普通文件");
        return;
    }
    QString fileName = item->text();
    m_strSaveFilePath = QFileDialog::getExistingDirectory(nullptr, "选择下载路径");
    if(m_strSaveFilePath.isEmpty()) return;
    m_strSaveFilePath = QString("%1/%2").arg(m_strSaveFilePath).arg(fileName);
    QMessageBox::information(this, "文件下载路径", m_strSaveFilePath);
//    qDebug() << m_strSaveFilePath;

    DownLoadFile(strCurPath, fileName, m_strSaveFilePath);


    // 请求下载strCurPath路径下的fileName文件
//    PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
//    pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
//    memcpy(pdu->cData, fileName.toStdString().c_str(), fileName.toStdString().size());
//    memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
//    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
//    free(pdu); pdu = nullptr;
}

void File::clickedShareFileBtn()
{
    if(m_pFileListW->currentItem() == nullptr) return;
    if(m_pFileListW->currentItem()->data(Qt::UserRole) != 1) {
        QMessageBox::information(this, "prompt", "目前仅支持分享普通文件");
        return;
    }
    m_strShareFileName = m_pFileListW->currentItem()->text();
    QListWidget *m_pFriendList = UserHomeWidget::getInstance()->getFriend()->getFriendList();
    ShareFile::getInstance()->updateFriendList(m_pFriendList);
    ShareFile::getInstance()->show();
//    for(int i = 0; i < m_pFriendList->count(); ++i) {
//        qDebug() << m_pFriendList->item(i)->text();
//    }
}

void File::clickedMoveFileBtn()
{
    QListWidgetItem *item = m_pFileListW->currentItem();
    if(item == nullptr) return;
    if(item->data(Qt::UserRole).toInt() != 1) {
        QMessageBox::information(this, "prompt", "该按钮仅支持移动普通文件");
        return;
    }
    m_strMoveFileName = item->text();
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    m_strMoveFilePath = QString("%1/%2").arg(strCurPath).arg(m_strMoveFileName);

    m_pMoveFileConfirmBtn->setEnabled(true);

}

void File::clickedMoveFileConfirmBtn()
{
    QListWidgetItem *item = m_pFileListW->currentItem();
    m_pMoveFileConfirmBtn->setEnabled(false);
    QString strCurPath = TcpClient::getInstance()->getCurrentFilePath();
    if(item == nullptr) { // 移动到当前文件夹下
        int pos = m_strMoveFilePath.lastIndexOf("/");
        QString prePath = m_strMoveFilePath.left(pos);
//        qDebug() << prePath << m_strMoveFilePath << strCurPath;
        if(prePath == strCurPath) {
            QMessageBox::information(this, "prompt", "该文件不能移动到原来的文件夹下");
            return;
        }
        m_strMoveDestDirPath = strCurPath;
    }
    else { // 移动到点击所选的文件夹下
        if(item->data(Qt::UserRole).toInt() != 0) {
            QMessageBox::information(this, "prompt", "该文件不是文件夹，不能移动");
            return;
        }
        QString strDestPath = item->text();
        m_strMoveDestDirPath = QString("%1/%2").arg(strCurPath).arg(strDestPath);
    }

    int filePathLen = m_strMoveFilePath.toStdString().size();
    int destPathLen = m_strMoveDestDirPath.toStdString().size();
    PDU *pdu = mkPDU(filePathLen + destPathLen + 2);
    pdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    // cData(char): filePathLen(int), destPathLen(int), m_strMoveFileName
    // cMsg(int): m_strMoveFilePath, m_strMoveDestDirPath
    qDebug() << m_strMoveFileName.toStdString().size() << m_strMoveFileName << "\n"
             << filePathLen << m_strMoveFilePath << "\n"
             << destPathLen << m_strMoveDestDirPath << "\n\n";
    memcpy(pdu->cData, &filePathLen, sizeof(int));
    memcpy(pdu->cData + sizeof(int), &destPathLen, sizeof(int));
    memcpy(pdu->cData + 2 * sizeof(int),
           m_strMoveFileName.toStdString().c_str(), m_strMoveFileName.toStdString().size());
    memcpy(pdu->cMsg, m_strMoveFilePath.toStdString().c_str(), filePathLen);
    memcpy((char*)(pdu->cMsg) + filePathLen + 1,
           m_strMoveDestDirPath.toStdString().c_str(), destPathLen);
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu); pdu = nullptr;
}



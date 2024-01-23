#include "tcpclient.h"
#include "ui_tcpclient.h"
#include <QDebug>
#include <QMutex>
#include <QGraphicsDropShadowEffect>
#include <QtConcurrent>
#include <QFuture>

#include "protocol.h"


TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    init_UI();
    loadConfig();


    qDebug() << "主线程:" << QThread::currentThread();

//    MySystemTrayFunc();

    m_TcpSocket = new QTcpSocket;
    m_TcpSocket->connectToHost(QHostAddress(m_strIP), m_usPort);

    m_pTestDelayTimer = new QTimer;
    m_pCalDelayTimer = new QElapsedTimer;
//    m_pTestDelayTimer->start(100);

    connect(m_pTestDelayTimer, &QTimer::timeout, this, &TcpClient::testDelay);


    connect(m_TcpSocket, &QTcpSocket::connected, this, &TcpClient::showConnect);
    connect(m_TcpSocket, &QTcpSocket::readyRead, this, &TcpClient::recv_Msg);
}

TcpClient::~TcpClient()
{
    delete ui;
}

TcpClient *TcpClient::self = 0;
TcpClient *TcpClient::getInstance()
{
    if(self == nullptr) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self == nullptr) {
            self = new TcpClient;
        }
    }
    return self;
}

void TcpClient::loadConfig()
{
    QString configPath = QString(":/config/client.ini");
    QSettings *pconfig = new QSettings(configPath, QSettings::IniFormat);
    m_strIP = pconfig->value("IP/server_ip").toString();
    m_usPort = pconfig->value("Port/port").toUInt();
    ui->label_IP->setText(m_strIP);
    ui->label_Port->setText(QString::number(m_usPort));
}

void TcpClient::init_UI()
{
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/res/tray.png"));
    ui->lineEdit_passWord->setEchoMode(QLineEdit::Password);

    ui->lineEdit_userName->setFocus();

    QString qss = "QWidget {"
                  "    background-color: #add8e6;"  // 偏蓝色
                  "    color: #333333;"
                  "    font-family: Arial, sans-serif;"
                  "}"

                  "QPushButton {"
                  "    background-color: #4682b4;"  // 按钮蓝色
                  "    color: white;"
                  "    border: none;"
                  "    padding: 15px 30px;"  // 增大按钮大小
                  "    font-size: 18px;"  // 指定字体大小
                  "    border-radius: 8px;"
                  "}"

                  "QPushButton:hover {"
                  "    background-color: #5f9ea0;"
                  "}";
    this->setStyleSheet(qss);

    // 创建阴影效果
//    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setBlurRadius(15);  // 调整阴影模糊半径
//    shadowEffect->setColor(QColor(0, 0, 0, 150));  // 调整阴影颜色和透明度
//    shadowEffect->setOffset(0, 0);  // 调整阴影偏移量
//    this->setGraphicsEffect(shadowEffect);

    connect(ui->pushButton_Close, &QPushButton::clicked, this, &TcpClient::deleteLater);
    connect(ui->pushButton_Minwindow, &QPushButton::clicked, this, &TcpClient::showMinimized);
    connect(ui->pushButton_Register, &QPushButton::clicked, this, &TcpClient::clickedBtnRegister);
    connect(ui->pushButton_Login, &QPushButton::clicked, this, &TcpClient::clickedBtnLogin);
    connect(ui->pushButton_Settings, &QPushButton::clicked, this, &TcpClient::clickedBtnSettings);
    connect(ui->pushButton_ConntecToServer, &QPushButton::clicked, this, &TcpClient::clickedBtnConnectToServer);

    // 监测单例类的信号
    connect(Settings::getInstance(), &Settings::updateSettings, this, &TcpClient::updateIPandPort);
}

QTcpSocket *TcpClient::getTcpSocket()
{
    return m_TcpSocket;
}

QString TcpClient::getMyUsrName()
{
    return m_myusrname;
}

QString TcpClient::getCurrentFilePath()
{
    return m_currentFilePath;
}

QStack<QString> &TcpClient::getFilePathStack()
{
    return m_DirPathStack; // 返回不能加&
}

QString TcpClient::getIP()
{
    return m_strIP;
}

qint16 TcpClient::getPort()
{
    return m_usPort;
}

void TcpClient::updateCurrentFilePath(QString str)
{
    m_currentFilePath = str;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this, "提示", "连接服务器成功");
}

void TcpClient::recv_Msg()
{
    if(m_bDownLoad) { // 接收数据
        QByteArray buffer = m_TcpSocket->readAll();
        m_file.write(buffer);
        m_lReceiveSize += buffer.size();
        ProgressBar::getInstance()->updateProgressUI(m_lReceiveSize * 100 / m_lTotalSize);

//        qDebug() << "接收 : " << buffer.size() << " 当前 : " << m_lReceiveSize;
        if(m_lReceiveSize >= m_lTotalSize) {
            m_file.close();
            m_bDownLoad = false;

            if(m_lReceiveSize == m_lTotalSize) {
                QMessageBox::information(this, "prompt", "download ok");
            }
            else {
                QMessageBox::information(this, "prompt", "download end, but not quite right: 文件里似乎多了些东西，但那似乎不影响");
            }
        }
        return;
    }


    uint uiPDULen = 0;
    m_TcpSocket->read((char*)&uiPDULen, sizeof(uint)); // 先接收sizeof(uint)字节大小，计算消息的总字节
    uint uiMsgLen = uiPDULen - sizeof(PDU);   // 实际消息大小
    PDU *pdu = mkPDU(uiMsgLen);  // 根据实际消息大小申请空间
    m_TcpSocket->read((char*)pdu + sizeof(int), uiPDULen - sizeof(uint)); //读取剩下的字节

    switch (pdu->uiMsgType) {
    case ENUM_MSG_TYPE_REGISTER_RESPOND:
    {
        if(strcmp(pdu->cData, REGISTER_OK) == 0) {
            QMessageBox::information(NULL, "提示", REGISTER_OK);
        }
        else {
            QMessageBox::information(NULL, "提示", REGISTER_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND: // 登录回复
    {
        if(strcmp(pdu->cData, LOGIN_OK) == 0) {
//            QMessageBox::information(NULL, "提示", LOGIN_OK);
            this->hide();
//            MySystemTrays->hide();
            UserHomeWidget::getInstance()->show();
//            UserHomeWidget::getInstance()->ShowSystemTray();
            m_currentFilePath = QString("./User/%1").arg(m_myusrname);
//            emit flushScreenSignal(); // 都安排好再触发信号
        }
        else {
            QMessageBox::information(NULL, "提示", LOGIN_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND: // 展示所有在线用户
    {
        UserHomeWidget::getInstance()->getFriend()->showAllOnlineUsr(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USER_RESPOND: // 搜索用户
    {
        QString SearchName = UserHomeWidget::getInstance()->getFriend()->m_SearchName;
        QString condition;
        if(strcmp(pdu->cData, SEARCH_USR_NO) == 0) condition = SEARCH_USR_NO;
        else if(strcmp(pdu->cData, SEARCH_USR_OFFLINE) == 0) condition = SEARCH_USR_OFFLINE;
        else condition = SEARCH_USR_ONLINE;

        QString str = QString("%1: %2").arg(SearchName).arg(condition);
        QMessageBox::information(this, "user condition", str);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST: // 收到由服务器转发的添加好友请求
    {
        char applicantName[STR_MAX_SIZE] = {0};
        strncpy(applicantName, pdu->cData + STR_MAX_SIZE, STR_MAX_SIZE); // applicationName 用户申请添加好友
        int res = QMessageBox::information(this, "提示", QString("%1 请求添加好友").arg(applicantName),
                                           QMessageBox::Yes, QMessageBox::No);
        // 把用户选择回复给服务器
        PDU *respdu = mkPDU(0);
        memcpy(respdu->cData, pdu->cData, STR_MAX_SIZE * 2); // 前边是要添加的用户名字，后边是申请者的名字
        if(res == QMessageBox::Yes) {
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
        }
        else {
            respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
        }
        m_TcpSocket->write((char*)respdu, respdu->uiPDULen);
        free(respdu);
        respdu = nullptr;
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND: // 处理添加好友的各种情况
    {
        QMessageBox::information(this, "prompt", QString(pdu->cData));
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:  // 同意添加好友的回复
    {
        QMessageBox::information(this, "提示", "对方同意添加您为好友");
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE: // 拒绝添加好友的回复
    {
        QMessageBox::information(this, "提示", "对方拒绝添加您为好友");
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND: // 刷新好友
    {
        UserHomeWidget::getInstance()->getFriend()->updateFriendList(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND: // 删除好友
    {
        if(strcmp(pdu->cData, DELETE_FRIEND_OK) == 0) {
            QMessageBox::information(this, "提示", "删除好友成功, 请刷新好友列表");
        }
        break;
    }
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST: // 收到由服务器转发的私聊请求
    {
        // 接收来自application用户的私聊请求
        char applicantName[STR_MAX_SIZE] = {};
        strncpy(applicantName, pdu->cData + STR_MAX_SIZE, STR_MAX_SIZE); // applicationName 用户发起聊天

        // 找到与application对话的私聊窗
        QList<PrivateChatWidget *> &m_pChatList = UserHomeWidget::getInstance()->getFriend()->getChatList();
        PrivateChatWidget *applicantChatWidget = nullptr;
        for(int i = 0; i < m_pChatList.size(); ++i) {
            QString name = m_pChatList.at(i)->getFrindName();
            if(name == QString(applicantName)) {
                applicantChatWidget = m_pChatList.at(i);
                break;
            }
        }
        // 没有与该用户聊天的widegt，新建一个
        if(applicantChatWidget == nullptr) {
            QMap<QString, bool> &m_vis = UserHomeWidget::getInstance()->getFriend()->getVisMap();
            m_vis.insert(QString(applicantName), true); // 防止关掉对话框后，点击application用户进行私聊，再新建一个对话框
            applicantChatWidget = new PrivateChatWidget;
            applicantChatWidget->setChatName(QString(applicantName));
            applicantChatWidget->setWindowTitle(QString("与%1的聊天框").arg(applicantName));
            m_pChatList.append(applicantChatWidget);
        }
        else {

        }
        applicantChatWidget->updateMsg(pdu);
        applicantChatWidget->show();

//        PrivateChatWidget::getInstance()->setChatName(QString(applicantName));
//        PrivateChatWidget::getInstance()->setWindowTitle(QString("好友 : %1\n").arg(applicantName));
//        if(PrivateChatWidget::getInstance()->isHidden()) {
//            PrivateChatWidget::getInstance()->show();
//        }
//        PrivateChatWidget::getInstance()->updateMsg(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST:  // 收到服务器转发的群聊请求，更新一下自己的群聊记录
    {
        UserHomeWidget::getInstance()->getFriend()->updateGroupMsg(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CREATE_DIR_RESPOND: // 创建新文件夹
    {
        QString promptStr;
        if(strcmp(pdu->cData, USER_CURRENT_DIR_NOT_EXIST) == 0) {
            promptStr.append(USER_CURRENT_DIR_NOT_EXIST);
        }
        else if(strcmp(pdu->cData, NEW_DIR_EXISTED) == 0) {
            promptStr.append(NEW_DIR_EXISTED);
        }
        promptStr.append(CREATE_NEW_DIR_OK);
        QMessageBox::information(this, "prompt", promptStr);
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_DIR_AND_FILE_RESPOND: // 刷新文件
    {
        UserHomeWidget::getInstance()->getFile()->updateFileList(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_DIR_RESPOND: // 删除文件夹
    {
        QString resStr = QString(pdu->cData);
        QMessageBox::information(this, "prompt", resStr);
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_RENAME_DIR_OR_FILE_RESPOND: // 重命名文件夹
    {
        QMessageBox::information(this, "prompt", QString(pdu->cData));
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_ENTER_DIR_RESPOND: // 进入选择的目录
    {
        if(strcmp(pdu->cData, ENTER_DIR_OK) == 0) {
            UserHomeWidget::getInstance()->getFile()->updateFileList(pdu);// 更新一下列表中的目录信息
            // 进入目录成功，更新一下当前目录
            m_DirPathStack.push(m_currentFilePath);
            m_currentFilePath = UserHomeWidget::getInstance()->getFile()->getNextDirectory();
            UserHomeWidget::getInstance()->getFile()->updateDirectoryPath(m_currentFilePath);
        }
        else {
            QMessageBox::information(this, "prompt", ENTER_DIR_FAILED);
        }
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND: // 上传文件
    {
        QMessageBox::information(this, "prompt", QString(pdu->cData));
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FILE_RESPOND: // 删除文件
    {
        QMessageBox::information(this, "prompt", pdu->cData);
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:  // 下载文件
    {
        QMessageBox::information(this, "prompt", pdu->cData);
        if(strcmp(pdu->cData, DOWNLOAD_FLLE_START) == 0) {
            qint64 fileSize;
            memcpy(&fileSize, (char*)(pdu->cMsg), sizeof(qint64));
//            qDebug() << "客户端要接收的文件大小 : " << fileSize;
            m_file.setFileName(UserHomeWidget::getInstance()->getFile()->getSaveFilePath());
            //以只写的方式打开文件，如果文件不存在，则会自动创建文件
            if(m_file.open(QIODevice::WriteOnly)) {
                m_bDownLoad = true;
                m_lTotalSize = fileSize;
                m_lReceiveSize = 0;
                ProgressBar::getInstance()->show();
            }
        }
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_REQUEST: // 接受好友发来的分享文件请求
    {
        char receiverName[STR_MAX_SIZE] = {}, senderName[STR_MAX_SIZE];
        memcpy(receiverName, pdu->cData, STR_MAX_SIZE);
        memcpy(senderName, pdu->cData + STR_MAX_SIZE, STR_MAX_SIZE);
        QString shareFilePath = QString((char*)pdu->cMsg);
//        qDebug() << shareFilePath;
        int pos = shareFilePath.lastIndexOf("/");
        QString fileName = shareFilePath.mid(pos + 1);
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "分享文件",
                              QString("您的好友%1分享文件%2，是否接收").arg(senderName).arg(fileName),
                              QMessageBox::Yes | QMessageBox::No);
        PDU *respdu = mkPDU(0);
        memcpy(respdu->cData, receiverName, STR_MAX_SIZE);
        memcpy(respdu->cData + STR_MAX_SIZE, senderName, STR_MAX_SIZE);
        if(reply == QMessageBox::Yes) {
            respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_AGREE;

            QString SaveFilePath = QFileDialog::getExistingDirectory(nullptr, "选择下载路径");
            if(SaveFilePath.isEmpty()) {
                respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REFUSE;
            }
            else {
                SaveFilePath = QString("%1/%2").arg(SaveFilePath).arg(fileName);
                QString strCurPath = shareFilePath.left(pos);
                UserHomeWidget::getInstance()->getFile()->DownLoadFile(strCurPath, fileName, SaveFilePath);
//                UserHomeWidget::getInstance()->getFile()->updateSaveFilePath(SaveFilePath);
//                // 用文件路径shareFilePath，向服务器请求下载文件
//                // 请求下载strCurPath路径下的fileName文件
//                QString strCurPath = shareFilePath.left(pos);
//    //            qDebug() << strCurPath;
//                PDU *pdu = mkPDU(strCurPath.toStdString().size() + 1);
//                pdu->uiMsgType = ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
//                memcpy(pdu->cData, fileName.toStdString().c_str(), fileName.toStdString().size());
//                memcpy(pdu->cMsg, strCurPath.toStdString().c_str(), strCurPath.toStdString().size());
//                TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
//                free(pdu); pdu = nullptr;
            }
        }
        else {
            respdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REFUSE;
        }
        m_TcpSocket->write((char*)respdu, respdu->uiPDULen);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND: // 接收分享文件的情况
    {
        QMessageBox::information(this, "prompt", pdu->cData);
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND: // 移动文件
    {
        QMessageBox::information(this, "prompt", QString(pdu->cData));
        emit flushScreenSignal();
        break;
    }
    case ENUM_MSG_TYPE_TEST_DELAY_RESPOND: // 接受服务器返回的测试延迟回复
    {
        qint64 latency = m_pCalDelayTimer->elapsed();
        qDebug() << "延迟：" << latency << "毫秒";
        UserHomeWidget::getInstance()->updateDelayShow(QString("%1 ms").arg(latency));
//        UserHomeWidget::getInstance()->StartTestDelayTimer();
        m_pTestDelayTimer->start(100);
        break;
    }
    default:
        break;
    }
    free(pdu);pdu = nullptr;
}

void TcpClient::sendmsg()
{
//    QString msg = ui->lineEdit_input->text();
//    PDU *pdu = mkPDU(msg.size());
//    memcpy(pdu->cMsg, msg.toStdString().c_str(), msg.size());
//    pdu->uiMsgType = 8888;
//    m_TcpSocket->write((char*)pdu, pdu->uiPDULen);
    //    pdu = nullptr;
}

void TcpClient::updateIPandPort()
{
    m_strIP = Settings::getInstance()->getIP();
    m_usPort = Settings::getInstance()->getPort().toUInt();
    ui->label_IP->setText(m_strIP);
    ui->label_Port->setText(QString::number(m_usPort));
}

void TcpClient::MySystemTrayFunc()
{
    QIcon qIcons(":/res/tray.png");
    MySystemTrays = new QSystemTrayIcon();
    MySystemTrays->setIcon(qIcons);


    qMiniAction = new QAction("最小化",this);
    connect(qMiniAction, &QAction::triggered, this, &TcpClient::hide);

    qRestoreAction=new QAction("显示",this);
    connect(qRestoreAction, &QAction::triggered, this, &TcpClient::showNormal);

    qExitAction=new QAction("退出",this);
    connect(qExitAction, &QAction::triggered, this, &TcpClient::deleteLater);


    pContextMenu = new QMenu(this); // new一个菜单对象
    // 将QActionc对象添加到菜单
    pContextMenu->addAction(qMiniAction);
    pContextMenu->addAction(qRestoreAction);
    pContextMenu->addAction(qExitAction);
    QFont font("Agency FB", 11);
    pContextMenu->setFont(font);


    QString menuQss = QString("QMenu::item:selected {"
                              "    background-color: lightgray;"
                              "    border: 1px solid gray;"
                              "    border-radius: 5px;"
                              "}");


    pContextMenu->setStyleSheet(menuQss);

    // 设置指定菜单为系统托盘的上下文菜单
    MySystemTrays->setContextMenu(pContextMenu);

    // 显示系统托盘
    MySystemTrays->show();
}

void TcpClient::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        clickedBtnLogin();
        return;
    }
    QWidget::keyPressEvent(event);
}

void TcpClient::clickedBtnRegister() // 注册
{
    QString userName = ui->lineEdit_userName->text();
    QString pwd = ui->lineEdit_passWord->text();
    if(userName.isEmpty() || pwd.isEmpty()) {
        QMessageBox::information(this, "Error", "请填写完整账号和密码");
        return;
    }
    if(userName.toStdString().size() > STR_MAX_SIZE - 1) {
        QMessageBox::information(this, "prompt", "用户名过长，请进行修改");
        return;
    }
    if(pwd.toStdString().size() > STR_MAX_SIZE - 1) {
        QMessageBox::information(this, "prompt", "密码过长，请进行修改");
        return;
    }
    PDU *pdu = mkPDU(0); // 只需要用固定部分
    pdu->uiMsgType = ENUM_MSG_TYPE_REGISTER_REQUEST; // 请求注册
    // 区别strncpy和strcpy，前者有第三个参数指定要复制的字节个数，后者没有
    strncpy(pdu->cData, userName.toStdString().c_str(), userName.toStdString().size());
    strncpy(pdu->cData + STR_MAX_SIZE, pwd.toStdString().c_str(), pwd.toStdString().size());
    m_TcpSocket->write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = nullptr;
}

void TcpClient::clickedBtnLogin() // 登录
{
    QString userName = ui->lineEdit_userName->text();
    QString pwd = ui->lineEdit_passWord->text();
    if(userName.isEmpty() || pwd.isEmpty()) {
        QMessageBox::information(this, "Error", "请填写完整账号和密码");
        return;
    }


    m_myusrname = userName;
    PDU *pdu = mkPDU(0); // 只需要用固定部分
    pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST; // 请求登录
    // 区别strncpy和strcpy，前者有第三个参数指定要复制的字节个数，后者没有
    strncpy(pdu->cData, userName.toStdString().c_str(), STR_MAX_SIZE);
    strncpy(pdu->cData + STR_MAX_SIZE, pwd.toStdString().c_str(), STR_MAX_SIZE);
    m_TcpSocket->write((char*)pdu, pdu->uiPDULen);
    free(pdu);pdu = nullptr;
}

void TcpClient::clickedBtnLogout()
{

}

void TcpClient::clickedBtnSettings()
{
    Settings::getInstance()->show();
}

void TcpClient::clickedBtnConnectToServer()
{
    m_TcpSocket->connectToHost(QHostAddress(m_strIP), m_usPort);
}

void TcpClient::testDelay()
{
    qDebug() << "向服务器发送测试延迟请求";
    m_pTestDelayTimer->stop();
    m_pCalDelayTimer->start();
    // 向服务器发送数据
    PDU *pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_TEST_DELAY_REQUEST;
    m_TcpSocket->write((char*)pdu, pdu->uiPDULen);
    free(pdu);pdu = nullptr;
}



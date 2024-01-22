#include "sharefile.h"
#include "ui_sharefile.h"
#include <QMutex>
#include "tcpclient.h"
#include "userhomewidget.h"

ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);

    this->setWindowTitle("分享文件");
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);

    m_font.setFamily("Agency FB");
    m_font.setPointSize(16);

    m_pFriendW = new QWidget;
    m_pButtonGroup = new QButtonGroup(m_pFriendW);
//    m_pButtonGroup->setExclusive(false);
    m_pVLayout = new QVBoxLayout(m_pFriendW);
    ui->scrollArea_ShowFriend->setWidget(m_pFriendW);



    test();

    connect(ui->pushButton_Close, &QPushButton::clicked, this, &ShareFile::close);
    connect(ui->pushButton_Min, &QPushButton::clicked, this, &ShareFile::showMinimized);
    connect(ui->pushButton_Cancel, &QPushButton::clicked, this, &ShareFile::close);
    connect(ui->pushButton_Confirm, &QPushButton::clicked, this, &ShareFile::clickedConfirmShareBtn);

    QString qss = "QWidget {"
                  "    background-color: #add8e6;"  // 偏蓝色
                  "    color: #333333;"
                  "    font-family: Arial, sans-serif;"
                  "}"

                  "QPushButton {"
                  "    background-color: #4682b4;"  // 按钮蓝色
                  "    color: white;"
                  "    border: none;"
                  "    padding: 5px 10px;"  // 增大按钮大小
                  "    font-size: 18px;"  // 指定字体大小
                  "    border-radius: 8px;"
                  "}"

                  "QPushButton:hover {"
                  "    background-color: #5f9ea0;"
                  "}";
    this->setStyleSheet(qss);
}



ShareFile::~ShareFile()
{
    delete ui;
}

ShareFile *ShareFile::self = 0;
ShareFile *ShareFile::getInstance()
{
    if(self == nullptr) {
        QMutex mutex;
        QMutexLocker lock(&mutex);
        if(self == nullptr) {
            self = new ShareFile;
        }
    }
    return self;
}

void ShareFile::updateFriendList(QListWidget *m_pFriendList)
{
    if(m_pFriendList == nullptr) return;
    m_pPreBtns = m_pButtonGroup->buttons();
    QAbstractButton *pBtn = nullptr;
    for(int i = 0; i < m_pPreBtns.count(); ++i) {
        pBtn = m_pPreBtns[i];
        m_pButtonGroup->removeButton(pBtn);
        m_pVLayout->removeWidget(pBtn);
        delete pBtn;
    }
    pBtn = nullptr;
    QCheckBox *pCheckBox = nullptr;
    for(int i = 0; i < m_pFriendList->count(); ++i) {
        pCheckBox = new QCheckBox(m_pFriendList->item(i)->text());
        pCheckBox->setFont(m_font);
        m_pVLayout->addWidget(pCheckBox);
        m_pButtonGroup->addButton(pCheckBox);
    }
}

void ShareFile::test()
{
    QCheckBox *pCheckBtn = nullptr;
    for(int i = 0; i < 15; ++i) {
        pCheckBtn = new QCheckBox(QString("jack%1").arg(i));
        pCheckBtn->setFont(m_font);
        m_pVLayout->addWidget(pCheckBtn);
        m_pButtonGroup->addButton(pCheckBtn);
    }
}

void ShareFile::clickedConfirmShareBtn()
{
    QString senderName = TcpClient::getInstance()->getMyUsrName();
    QString shareFileDir = TcpClient::getInstance()->getCurrentFilePath();
    QString shareFileName = UserHomeWidget::getInstance()->getFile()->getShareFileName();
    QString shareFilePath = shareFileDir + "/" + shareFileName;
    QString receiverName = "";
    m_pPreBtns = m_pButtonGroup->buttons();
    for(int i = 0; i < m_pPreBtns.count(); ++i) {
        if(m_pPreBtns[i]->isChecked()) {
            receiverName = m_pPreBtns[i]->text();
            break;
        }
    }
    if(receiverName.isEmpty()) {
        QMessageBox::information(this, "prompt", "请选择一个用户进行分享");
        return;
    }
    PDU *pdu = mkPDU(shareFilePath.toStdString().size() + 1);
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    memcpy(pdu->cData, receiverName.toStdString().c_str(), receiverName.toStdString().size());
    memcpy(pdu->cData + 32, senderName.toStdString().c_str(), senderName.toStdString().size());
    memcpy(pdu->cMsg, shareFilePath.toStdString().c_str(), shareFilePath.toStdString().size());
    TcpClient::getInstance()->getTcpSocket()->write((char*)pdu, pdu->uiPDULen);
    free(pdu);pdu = nullptr;
    this->close();
}

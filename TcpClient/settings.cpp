#include "settings.h"
#include "ui_settings.h"
#include <QMutex>
#include <QMessageBox>
#include <QHostAddress>

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    init_UI();

    connect(ui->pushButton_Close, &QPushButton::clicked, this, &Settings::close);
    connect(ui->pushButton_Confirm, &QPushButton::clicked, this, &Settings::clickedBtnConfirmSettings);
}

Settings::~Settings()
{
    delete ui;
}

Settings* Settings::self = 0;
Settings *Settings::getInstance()
{
    if(self == 0) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self == 0) {
            self = new Settings;
        }
    }
    return self;
}

void Settings::init_UI()
{
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setWindowTitle("设置");
    QSize IconSize(25, 25);      // 图标大小
    ui->pushButton_Close->setIconSize(IconSize);
    ui->pushButton_Close->setIcon(QIcon(":/res/close.png"));
    ui->pushButton_Confirm->setIconSize(IconSize);

    ui->lineEdit_IP->setText("43.140.214.161");
    ui->lineEdit_Port->setText("8888");

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
}

QString Settings::getIP()
{
    return m_strIP;
}

QString Settings::getPort()
{
    return m_strPort;
}

void Settings::clickedBtnConfirmSettings()
{
    if(ui->lineEdit_IP->text().isEmpty() || ui->lineEdit_Port->text().isEmpty()) {
        QMessageBox::information(this, "提示", "请输入完整配置信息");
        return;
    }
    QHostAddress address(ui->lineEdit_IP->text());
    if(address.protocol() == QAbstractSocket::UnknownNetworkLayerProtocol) {
        QMessageBox::information(this, "提示", "请输入合法的IP地址");
        return;
    }
    if(ui->lineEdit_Port->text().toInt() < 1024 || ui->lineEdit_Port->text().toInt() > 49151) {
        QMessageBox::information(this, "提示", "请输入合法的端口号");
        return;
    }
    m_strIP = ui->lineEdit_IP->text();
    m_strPort = ui->lineEdit_Port->text();
    emit updateSettings();
    this->close();
}

#include "tcpserver.h"
//#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
//    , ui(new Ui::TcpServer)
{
//    ui->setupUi(this);
    loadConfig();

    // 如果当前目录里没有User目录，就创建User目录
    QDir dir;
    if(!dir.exists(QString("./User"))) {
        dir.mkdir(QString("./User"));
    }
    MyTcpServer::getInstance()->listen(QHostAddress::Any, m_usPort);
//    MyTcpServer::getInstance()->listen(QHostAddress(m_strIP), m_usPort);
}


TcpServer::~TcpServer()
{
//    delete ui;
}

void TcpServer::loadConfig()
{
    QString configPath = QString(":/config/server.ini");
    QSettings *pconfig = new QSettings(configPath, QSettings::IniFormat);
    m_strIP = pconfig->value("IP/client_ip").toString();
    m_usPort = pconfig->value("Port/port").toUInt();
}


#include "userhomewidget.h"
#include <QMutex>
#include "tcpclient.h"

UserHomeWidget::UserHomeWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle(TcpClient::getInstance()->getMyUsrName());
    this->resize(1200, 800);
    this->setProperty("canMove", true);
    this->setWindowFlag(Qt::FramelessWindowHint);

//    MySystemTrayFunc(); // 创建托盘
//    MySystemTrays->show();


    // 界面上部布局
    QHBoxLayout *pHTopLayout = new QHBoxLayout;
    QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    QSize IconSize(25, 25);      // 图标大小
    QFont font("Agency FB", 16);

    m_CloseBtn = new QPushButton;
    m_MinScreenBtn = new QPushButton;
    m_CloseBtn->resize(IconSize);
    m_MinScreenBtn->resize(IconSize);
    m_CloseBtn->setIcon(QIcon(":/res/close.png"));
    m_MinScreenBtn->setIcon(QIcon(":/res/minwindow.png"));
    m_CloseBtn->setIconSize(IconSize);
    m_MinScreenBtn->setIconSize(IconSize);

    m_ShowUserNameLabel = new QLabel;
    m_ShowDelayLabel = new QLabel;
    QString ShowUserNameStr = QString("用户名 : %1").arg(TcpClient::getInstance()->getMyUsrName());
    m_ShowUserNameLabel->setText(ShowUserNameStr);
    m_ShowUserNameLabel->setFont(font);
    m_ShowDelayLabel->setFont(font);
    pHTopLayout->addWidget(m_ShowUserNameLabel);
    pHTopLayout->addSpacerItem(spacer);
    pHTopLayout->addWidget(m_ShowDelayLabel);
    pHTopLayout->addSpacerItem(spacer);
    pHTopLayout->addWidget(m_MinScreenBtn);
    pHTopLayout->addWidget(m_CloseBtn);


    // 左边的选项
    m_plistWidget = new QListWidget();
    m_plistWidget->setFixedWidth(135);
    m_plistWidget->setFont(font);
    m_plistWidget->addItem("好友");
    m_plistWidget->addItem("文件");

    // friend界面
    m_pFriend = new Friend;
    // file界面
    m_pFile = new File;

    // 存界面，在右边放出来
    m_pstackWidget = new QStackedWidget;
    m_pstackWidget->addWidget(m_pFriend);
    m_pstackWidget->addWidget(m_pFile);

    // 选界面水平布局一下
    QHBoxLayout *pMain_HLayout = new QHBoxLayout;
    pMain_HLayout->addWidget(m_plistWidget);
    pMain_HLayout->addWidget(m_pstackWidget);

    // 跟最上边垂直布局一下
    QVBoxLayout *pMain = new QVBoxLayout;
    pMain->addLayout(pHTopLayout);
    pMain->addLayout(pMain_HLayout);


    m_Frame = new QFrame();

    m_Frame->setLayout(pMain);
    m_Frame->setFrameShape(QFrame::Box);
    this->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *pVBMain = new QVBoxLayout;
    pVBMain->addWidget(m_Frame);
    setLayout(pVBMain);


    QString qss = "QWidget {"
                  "    background-color: #add8e6;"  // 偏蓝色
                  "    color: #333333;"
                  "    font-family: Arial, sans-serif;"
                  "}"

                  "QPushButton {"
                  "    background-color: #4682b4;"  // 按钮蓝色
                  "    color: white;"
                  "    border: none;"
                  "    padding: 8px 15px;"  // 增大按钮大小
                  "    font-size: 18px;"  // 指定字体大小
                  "    border-radius: 8px;"
                  "}"

                  "QPushButton:hover {"
                  "    background-color: #5f9ea0;"
                  "}";
    this->setStyleSheet(qss);

    // 这里的关闭窗口只能用close，deleteLater会程序异常结束
    connect(m_CloseBtn, &QPushButton::clicked, this, &UserHomeWidget::close);
    connect(m_MinScreenBtn, &QPushButton::clicked, this, &UserHomeWidget::showMinimized);
    connect(m_plistWidget, &QListWidget::currentRowChanged, m_pstackWidget, &QStackedWidget::setCurrentIndex);
}

UserHomeWidget::~UserHomeWidget()
{
    qDebug() << "操作界面析构";
}

UserHomeWidget* UserHomeWidget::self = 0;
UserHomeWidget *UserHomeWidget::getInstance()
{
    if(self == nullptr) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if(self == nullptr) {
            self = new UserHomeWidget;
        }
    }
    return self;
}

Friend *UserHomeWidget::getFriend()
{
    return m_pFriend;
}

File *UserHomeWidget::getFile()
{
    return m_pFile;
}

void UserHomeWidget::updateDelayShow(QString delay)
{
    m_ShowDelayLabel->setText(delay);
}

void UserHomeWidget::ShowSystemTray()
{
    // 显示系统托盘
    MySystemTrays->show();
}

void UserHomeWidget::MySystemTrayFunc()
{
    QIcon qIcons(":/res/tray.png");
    MySystemTrays = new QSystemTrayIcon();
    MySystemTrays->setIcon(qIcons);


    qMiniAction = new QAction("最小化");
    connect(qMiniAction, &QAction::triggered, this, &UserHomeWidget::hide);

    qRestoreAction=new QAction("显示");
    connect(qRestoreAction, &QAction::triggered, this, &UserHomeWidget::showNormal);

    qExitAction=new QAction("退出");
    connect(qExitAction, &QAction::triggered, this, &UserHomeWidget::deleteLater);


    pContextMenu = new QMenu(); // new一个菜单对象
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
}




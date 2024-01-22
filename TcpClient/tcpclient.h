#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QSettings>
#include <QMouseEvent>
#include <QMessageBox>

#include <QTcpSocket>
#include <QHostAddress>

#include <QMap>
#include <QFileDevice>
#include <QStack>
#include <QTimer>
#include <QElapsedTimer>

// 系统托盘相关的头文件
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>


#include "protocol.h"
#include "userhomewidget.h"
#include "settings.h"
#include "progressbar.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
// 定义成单例模式，提供了一个全局的访问点，允许在整个应用程序中访问该实例
// 这样就可以很方便的调用tcpsocket
    static TcpClient* getInstance();

    void loadConfig();
    void init_UI();

    QTcpSocket *getTcpSocket(); // 提供一个全局调用tcpsocket进行通信的接口

    QString getMyUsrName();
    QString getCurrentFilePath();
    QStack <QString> &getFilePathStack(); // 返回时变量不能加&

    QString getIP();
    qint16 getPort();

    void updateCurrentFilePath(QString str);



signals:
    void flushScreenSignal();

public slots:
    void showConnect(); // 显示连接成功
    void recv_Msg();
    void sendmsg();
    void updateIPandPort(); // 使用设置修改IP和Port


    void MySystemTrayFunc(); // 系统托盘函数

protected:
    void keyPressEvent(QKeyEvent *event) override;


private slots:
    void clickedBtnRegister();
    void clickedBtnLogin();
    void clickedBtnLogout();
    void clickedBtnSettings();
    void clickedBtnConnectToServer();

    void testDelay();


private:
    Ui::TcpClient *ui;

    static TcpClient *self;

    QString m_strIP;
    quint16 m_usPort; //无符号的，端口号范围是2字节

    QTcpSocket *m_TcpSocket;

    QString m_myusrname;

    QString m_currentFilePath; // 在登录成功后设置当前用户文件夹目录

    QStack<QString> m_DirPathStack;


    bool m_bDownLoad = false; // 标记是否处于下载状态
    QFile m_file;
    qint64 m_lTotalSize;   // 总共要接收的数据
    qint64 m_lReceiveSize; // 已经接收的数据

    QTimer *m_pTestDelayTimer;
    QElapsedTimer *m_pCalDelayTimer;

    // 系统托盘
    QSystemTrayIcon *MySystemTrays; // 托盘指针
    QMenu *pContextMenu; // 托盘菜单

    QAction *qMiniAction;       // 客户端最小化
    QAction *qRestoreAction;    // 恢复客户端
    QAction *qExitAction;       // 退出系统
};
#endif // TCPCLIENT_H

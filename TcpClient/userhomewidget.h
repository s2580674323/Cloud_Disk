#ifndef USERHOMEWIDGET_H
#define USERHOMEWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>

#include <QTimer>
#include <QElapsedTimer>

// 系统托盘相关的头文件
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>


#include "friend.h"
#include "file.h"

class UserHomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserHomeWidget(QWidget *parent = nullptr);
    ~UserHomeWidget();

    static UserHomeWidget* getInstance(); // 单例


    Friend *getFriend();
    File *getFile();// 通过本单例来获取里边的类，进行操作

    void updateDelayShow(QString delay);

    void ShowSystemTray();

    void StartTestDelayTimer();

private slots:
    void MySystemTrayFunc(); // 系统托盘函数


signals:

private:
    static UserHomeWidget *self; // 设计一个单例模式

    QPushButton *m_CloseBtn;
    QPushButton *m_MinScreenBtn;
    QLabel *m_ShowUserNameLabel;
    QLabel *m_ShowDelayLabel;

    QFrame *m_Frame;

    QListWidget *m_plistWidget; // 左边的选项

    Friend *m_pFriend; // friend界面
    File *m_pFile; // 文件操作界面

    QStackedWidget *m_pstackWidget; // 存一下界面，根据选项切换界面


    // 系统托盘
    QSystemTrayIcon *MySystemTrays; // 托盘指针
    QMenu *pContextMenu; // 托盘菜单

    QAction *qMiniAction;       // 客户端最小化
    QAction *qRestoreAction;    // 恢复客户端
    QAction *qExitAction;       // 退出系统
};

#endif // USERHOMEWIDGET_H

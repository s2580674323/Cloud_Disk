#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMap>

#include "online.h"
#include "privatechatwidget.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = nullptr);

    void showAllOnlineUsr(PDU *pdu);
    void updateFriendList(PDU *pdu);
    void updateGroupMsg(PDU *pdu);

    QList<PrivateChatWidget *> &getChatList();// 返回时变量不能加&
    QMap <QString, bool> &getVisMap();// 返回时变量不能加&
    QListWidget *getFriendList();

    QString m_SearchName;

private slots:
    void clickedShowOnlineBtn();
    void clickedSearchUserBtn();
    void clickedFlushFriendBtn();
    void clickedDeleteFriendBtn();
    void clickedPrivateChatBtn();
    void clickedGroupChatBtn();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:

private:
    QTextEdit *m_pShowMsgTE;
    QListWidget *m_pFriendListWidget;
    QLineEdit *m_pInputMsgLE;

    QLabel *m_ShowMsgHistory;
    QLabel *m_ShowFriendList;

    QPushButton *m_pDeleteFriendBtn;
    QPushButton *m_pFlushFriendBtn;
    QPushButton *m_pShowOnlineUserBtn;
    QPushButton *m_pSearchUserBtn;
    QPushButton *m_pSendMsgBtn;
    QPushButton *m_pPrivateChatBtn;

    Online *m_pOnline;

    QList<PrivateChatWidget *> m_pChatList;
    QMap <QString, bool> m_vis;
};

#endif // FRIEND_H

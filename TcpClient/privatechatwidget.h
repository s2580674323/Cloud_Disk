#ifndef PRIVATECHATWIDGET_H
#define PRIVATECHATWIDGET_H

#include <QWidget>
#include "protocol.h"

namespace Ui {
class PrivateChatWidget;
}

class PrivateChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChatWidget(QWidget *parent = nullptr);
    ~PrivateChatWidget();

//    static PrivateChatWidget* getInstance();

    QString getFrindName();
    void setChatName(QString friName);
    void updateMsg(const PDU *pdu);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void clickedBtnSendMessage();
private:
    Ui::PrivateChatWidget *ui;
//    static PrivateChatWidget *self;

    QString m_friName, m_myName;
};

#endif // PRIVATECHATWIDGET_H

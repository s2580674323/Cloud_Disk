#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include "protocol.h"


namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    ~Online();

    void ShowUsr(PDU *pdu);

private slots:
    void AddFriend();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H

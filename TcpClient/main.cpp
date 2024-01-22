#include "appinitdrag.h"
#include <QApplication>

#include "tcpclient.h"

#include "online.h"
#include "friend.h"
#include "userhomewidget.h"
#include "privatechatwidget.h"
#include "sharefile.h"
#include "progressbar.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AppInitDrag::GetInstance()->start();

//    Online ww;
//    ww.show();

//    Friend ww;
//    ww.show();

//    UserHomeWidget ww;
//    ww.show();

//    PrivateChatWidget ww;
//    ww.show();

//    ShareFile ww;
//    ww.show();

//    ProgressBar ww;
//    ww.show();

    TcpClient::getInstance()->show();
//    UserHomeWidget::getInstance()->show();
    return a.exec();
}

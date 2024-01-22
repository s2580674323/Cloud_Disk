#include "tcpserver.h"
#include "usedb.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    useDB::getInstance()->init();
    TcpServer w;
    w.show();
    return a.exec();
}

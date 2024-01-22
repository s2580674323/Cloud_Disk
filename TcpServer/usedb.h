#ifndef USEDB_H
#define USEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QStringList>

// 设计成单例模式
// 对于字符串的数据的占位符记得加引号

class useDB : public QObject
{
    Q_OBJECT
public:
    explicit useDB(QObject *parent = nullptr);

    static useDB* getInstance();
    void init();

    bool Register(const char *name, const char *pwd);
    bool Login(const char *name, const char *pwd);
    void Offline(const char *name);
    QStringList ShowAllOnline();
    int SearchUsr(const char *name);
    int AddFriend(const char *friname, const char *myname);
    void UpdateAgreeAddFriend(const char *friname, const char *myname);
    QStringList FlushFriend(const char *name);
    bool DeleteFriend(const char *friname, const char *myname);

private:
    static useDB *instance;
    QSqlDatabase m_db; //连接数据库
};

#endif // USEDB_H

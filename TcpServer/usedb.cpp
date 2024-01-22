#include "usedb.h"
#include <QDebug>

useDB::useDB(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}

useDB* useDB::instance = new useDB;

useDB *useDB::getInstance()
{
    return instance;
}

void useDB::init()
{
    QStringList list = QSqlDatabase::drivers();
    qDebug() << list;

     m_db.setHostName("127.0.0.1"); // 本地连接
     m_db.setPort(3306);       // 如果使用的是默认端口可以不设置
     m_db.setUserName("root");	// 数据库用户名
     m_db.setPassword("330030"); // 数据库密码
     m_db.setDatabaseName("networkdiskdb"); // 数据库名字


    if(!m_db.open()) {
        QMessageBox::information(NULL, "打开数据库失败", "打开数据库失败"); // 这里不能填this
    }


    QString createUserInfo = QString("create table if not exists userinfo(id int primary key auto_increment,"
                             "name varchar(32) unique,"
                             "pwd varchar(32),"
                             "online integer default 0);"); // online禁止重复登录
    QString createFriend = QString("create table if not exists friend(id int, friendId int,"
                                   "primary key(id, friendId));");
    QString insertinfo = QString("insert ignore into userinfo(name, pwd) values('sqd', 'sqd'), ('sss', 'sss');");


    QSqlQuery query(m_db);

    query.exec(createUserInfo);
    query.exec(createFriend);
    query.exec(insertinfo);
    query.exec(QString("select * from userinfo"));
    while(query.next()) {
        qDebug() << query.value(0).toInt() << " " << query.value(1).toString()  << query.value(2).toString()
                 << " " << query.value(3).toInt();
    }
//    query.exec(insertinfo);

//    QString str1 = QString("delete from friend where id = "
//                           "(select id from userinfo where name = '%1') "
//                           "and friendId = "
//                           "(select id from userinfo where name = '%2')"
//                           ).arg("tom").arg("sss");
//    bool ok = query.exec(str1);
//    qDebug() << ok;
}

bool useDB::Register(const char *name, const char *pwd)
{
    if(name == nullptr || pwd == nullptr) return false;
    QString strinsert = QString("insert into userinfo(name, pwd) values('%1', '%2')").arg(name).arg(pwd);
    QSqlQuery query(m_db);
    bool res = query.exec(strinsert);
    return res;
}

bool useDB::Login(const char *name, const char *pwd)
{
    if(name == nullptr || pwd == nullptr) return false;
    QString strselect = QString("select * from userinfo where name = '%1' and pwd = '%2'"
                                "and online = 0").arg(name).arg(pwd);
    QSqlQuery query(m_db);
    query.exec(strselect);
    if(query.next()) {
        QString update_online_status = QString("update userinfo set online = 1 " // 换行时记得留一个空格
                                               "where name = '%1' and pwd = '%2'").arg(name).arg(pwd);
        query.exec(update_online_status);
        return true;
    }
    return false;
}

void useDB::Offline(const char *name)
{
    if(name == nullptr) {
        return;
    }
    // name是unique类型的
    QString str = QString("update userinfo set online = 0 where name = '%1'").arg(name);
    QSqlQuery query(m_db);
    query.exec(str);
}

QStringList useDB::ShowAllOnline()
{
    QString str = QString("select name from userinfo where online = 1");
    QSqlQuery query(m_db);
    query.exec(str);
    QStringList res;
    while(query.next()) {
        res.append(query.value(0).toString());
    }
    return res;
}

int useDB::SearchUsr(const char *name)
{
    if(name == nullptr) return -1;
    QString str = QString("select online from userinfo where name = '%1'").arg(name);
    QSqlQuery query(m_db);
    query.exec(str);
    if(query.next()) { // 必须得用next移动到起始位置
        return query.value(0).toInt();
    }
    else return -1;
}

int useDB::AddFriend(const char *friname, const char *myname)
{
    if(myname == nullptr || friname == nullptr) {
        return -1;
    }
    QString str = QString("select * from friend where id = (select id from userinfo where name = '%1') "
                          "and friendId = (select id from userinfo where name = '%2') "
                          "or "
                          "id = (select id from userinfo where name = '%3') "
                          "and friendId = (select id from userinfo where name = '%4')")
            .arg(myname).arg(friname).arg(friname).arg(myname);
    QSqlQuery query(m_db);
    query.exec(str);
    if(query.next()) {
        return -2;// 已经是好友了
    }
    str = QString("select online from userinfo where name = '%1'").arg(friname);
    query.exec(str);
    int res = 0;
    if(query.next()) {
        res = query.value(0).toInt();
    }
    return res;
}

void useDB::UpdateAgreeAddFriend(const char *friname, const char *myname)
{
    if(myname == nullptr || friname == nullptr) return;
    QString str = QString("insert ignore into friend(id, friendId) values("
                          "(select id from userinfo where name = '%1'), "
                          "(select id from userinfo where name = '%2')"
                          ")")
            .arg(myname).arg(friname);
    QSqlQuery query(m_db);
    query.exec(str);
}

QStringList useDB::FlushFriend(const char *name)
{
    QStringList strList_AllFriend;
    if(name == nullptr) return strList_AllFriend;
    QString str = QString("select name from userinfo where id in ("
                          "select friendId from friend where id = "
                          "(select id from userinfo where name = '%1') "
                          "union "
                          "select id from friend where friendId = "
                          "(select id from userinfo where name = '%2')"
                      ") and online = 1").arg(name).arg(name);
    QSqlQuery query(m_db);
    query.exec(str);
    while(query.next()) {
        QString now_name = query.value(0).toString();
        strList_AllFriend.append(now_name);
    }
    return strList_AllFriend;
}

bool useDB::DeleteFriend(const char *friname, const char *myname)
{
    if(friname == nullptr || myname == nullptr || friname == myname) return false;
    QString str1 = QString("delete from friend where id = "
                           "(select id from userinfo where name = '%1') "
                           "and friendId = "
                           "(select id from userinfo where name = '%2')"
                           ).arg(friname).arg(myname);
    QString str2 = QString("delete from friend where id = "
                           "(select id from userinfo where name = '%1') "
                           "and friendId = "
                           "(select id from userinfo where name = '%2')"
                           ).arg(myname).arg(friname);
    QSqlQuery query(m_db);
    return query.exec(str1) & query.exec(str2);
}

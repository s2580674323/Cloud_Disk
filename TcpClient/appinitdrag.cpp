#include "appinitdrag.h"

AppInitDrag::AppInitDrag()
{

}
AppInitDrag *AppInitDrag::self = 0;
AppInitDrag *AppInitDrag::GetInstance()
{// 双重检查锁定机制(Double-Checked Locking)的单例模式
    if(self == nullptr) {
        // 避免多个线程同时通过锁定和创建实例
        QMutex mutex; // 用于实现互斥锁, 确保多线程环境下对 self 的访问是安全的
// QMutexLocker 是Qt提供的辅助类，它简化了对QMutex的使用，通过在其初始化时自动锁定和解锁互斥锁
        QMutexLocker locker(&mutex); // RAII 风格的锁定对象，能确保只有一个线程能够持有 mutex
        if(self == nullptr) {
            // 在锁定的情况下再次检查 self 是否为 nullptr，以防止多个线程同时通过第一次检查
            self = new AppInitDrag;
        }
    }
    return self;
}

void AppInitDrag::start()
{
/*
QApplication（通常简写为qApp）是Qt框架中的一个核心类，用于管理应用程序的控制流和全局设置。
它是Qt应用程序的主要入口点，并提供了一些基本的应用程序功能，包括事件处理、国际化支持、应用程序广播以及整个应用程序的全局设置
*/
//将 this 注册为事件过滤器，从而捕捉特定的鼠标事件
    qApp->installEventFilter(this); // 安装事件过滤器
}

// 使得具有 canMove 属性的 QWidget 窗口能够通过鼠标左键拖动来移动的功能
bool AppInitDrag::eventFilter(QObject *obj, QEvent *event)
{
    QWidget *w = (QWidget *)obj;
    if (!w->property("canMove").toBool()) {
        return QObject::eventFilter(obj, event);
    }

    static QPoint mousePoint;
    static bool mousePressed = false;

    QMouseEvent *mevent = static_cast<QMouseEvent *>(event);
    if (mevent->type() == QEvent::MouseButtonPress) {
        if (mevent->button() == Qt::LeftButton) {
            mousePressed = true;
            mousePoint = mevent->globalPos() - w->pos();
            return true;
        }
    } else if (mevent->type() == QEvent::MouseButtonRelease) {
        mousePressed = false;
        return true;
    } else if (mevent->type() == QEvent::MouseMove) {
        if (mousePressed && mevent->buttons() == Qt::LeftButton) {
            w->move(mevent->globalPos() - mousePoint);
            return true;
        }
    }

    return QObject::eventFilter(obj, event);
}

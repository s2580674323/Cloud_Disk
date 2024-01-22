#ifndef APPINITDRAG_H
#define APPINITDRAG_H

#include <QObject>
#include <QMutex>
#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class AppInitDrag : public QObject
{
    Q_OBJECT
public:
    AppInitDrag();
    static AppInitDrag* GetInstance();
    void start();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    static AppInitDrag *self;
};

#endif // APPINITDRAG_H

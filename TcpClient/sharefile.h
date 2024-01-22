#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include <QButtonGroup>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QFont>

namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();
    static ShareFile *getInstance();

    void updateFriendList(QListWidget *m_pFriendList);
    void test();

private slots:
    void clickedConfirmShareBtn();
private:
    Ui::ShareFile *ui;
    static ShareFile *self;

    QButtonGroup *m_pButtonGroup;
    QWidget *m_pFriendW;
    QVBoxLayout *m_pVLayout;
    QList<QAbstractButton *> m_pPreBtns;

    QFont m_font;
};

#endif // SHAREFILE_H

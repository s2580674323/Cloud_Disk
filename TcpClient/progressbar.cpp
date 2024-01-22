#include "progressbar.h"
#include "tcpclient.h"
#include <QMutex>

ProgressBar::ProgressBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(400, 200);
    this->setWindowTitle("进度");
    m_pProgressBar = new QProgressBar;

    m_pProgressBar->setRange(0, 100);

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pProgressBar);

    this->setLayout(pMain);
}

ProgressBar *ProgressBar::self = 0;
ProgressBar *ProgressBar::getInstance()
{
    if(self == nullptr) {
        QMutex mutex;
        QMutexLocker lock(&mutex);
        if(self == nullptr) {
            self = new ProgressBar;
        }
    }
    return self;
}


void ProgressBar::updateProgressUI(int percent)
{
    m_pProgressBar->setValue(percent);
}

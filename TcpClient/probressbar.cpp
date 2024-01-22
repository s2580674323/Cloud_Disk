#include "probressbar.h"
#include "tcpclient.h"
#include <QMutex>

ProbressBar::ProbressBar(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(400, 200);
    this->setWindowTitle("进度");
    m_pProgressBar = new QProgressBar;
    m_pShowProgressLabel = new QLabel();
    m_pShowProgressLabel->setText(QString("0%"));

    QHBoxLayout *pMain = new QHBoxLayout;
    pMain->addWidget(m_pProgressBar);
    pMain->addWidget(m_pShowProgressLabel);

    this->setLayout(pMain);
}

ProbressBar *ProbressBar::self = 0;
ProbressBar *ProbressBar::getInstance()
{
    if(self == nullptr) {
        QMutex mutex;
        QMutexLocker lock(&mutex);
        if(self == nullptr) {
            self = new ProbressBar;
        }
    }
    return self;
}

void ProbressBar::resetProgressUI(qint64 TotalSize)
{
    m_pShowProgressLabel->setText(("0%"));
    m_pProgressBar->setMaximum(TotalSize);
    m_pProgressBar->setValue(0);
}

void ProbressBar::updateProgressUI(qint64 ReceivedSize)
{
    int percent = ReceivedSize / m_pProgressBar->maximum();
    m_pShowProgressLabel->setText(QString("%1 %").arg(percent));
    m_pProgressBar->setValue(ReceivedSize);
}

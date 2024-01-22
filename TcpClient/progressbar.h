#ifndef PROBRESSBAR_H
#define PROBRESSBAR_H

#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class ProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBar(QWidget *parent = nullptr);

    static ProgressBar* getInstance();

    void updateProgressUI(int percent);
signals:

private:
    static ProgressBar *self;

    QProgressBar *m_pProgressBar;
};

#endif // PROBRESSBAR_H

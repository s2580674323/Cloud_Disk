#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    static Settings* getInstance();

    void init_UI();

    QString getIP();
    QString getPort();
private slots:
    void clickedBtnConfirmSettings();

signals:
    void updateSettings();

private:
    Ui::Settings *ui;

    static Settings *self;
    QString m_strIP;
    QString m_strPort;
};

#endif // SETTINGS_H

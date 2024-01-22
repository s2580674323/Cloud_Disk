/********************************************************************************
** Form generated from reading UI file 'tcpclient.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TCPCLIENT_H
#define UI_TCPCLIENT_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TcpClient
{
public:
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_Settings;
    QPushButton *pushButton_Minwindow;
    QPushButton *pushButton_Close;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label_UserName;
    QLineEdit *lineEdit_userName;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_Password;
    QLineEdit *lineEdit_passWord;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *pushButton_Register;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_Login;
    QSpacerItem *horizontalSpacer_9;
    QSpacerItem *verticalSpacer_4;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_ConntecToServer;
    QSpacerItem *horizontalSpacer_12;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_IP;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_Logout;
    QSpacerItem *horizontalSpacer_11;
    QLabel *label_Port;

    void setupUi(QWidget *TcpClient)
    {
        if (TcpClient->objectName().isEmpty())
            TcpClient->setObjectName(QString::fromUtf8("TcpClient"));
        TcpClient->resize(522, 491);
        frame = new QFrame(TcpClient);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 521, 491));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButton_Settings = new QPushButton(frame);
        pushButton_Settings->setObjectName(QString::fromUtf8("pushButton_Settings"));
        pushButton_Settings->setMinimumSize(QSize(50, 50));
        pushButton_Settings->setMaximumSize(QSize(50, 50));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Settings->setIcon(icon);
        pushButton_Settings->setIconSize(QSize(50, 50));

        horizontalLayout_3->addWidget(pushButton_Settings);

        pushButton_Minwindow = new QPushButton(frame);
        pushButton_Minwindow->setObjectName(QString::fromUtf8("pushButton_Minwindow"));
        pushButton_Minwindow->setMinimumSize(QSize(50, 50));
        pushButton_Minwindow->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/minwindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Minwindow->setIcon(icon1);
        pushButton_Minwindow->setIconSize(QSize(50, 50));

        horizontalLayout_3->addWidget(pushButton_Minwindow);

        pushButton_Close = new QPushButton(frame);
        pushButton_Close->setObjectName(QString::fromUtf8("pushButton_Close"));
        pushButton_Close->setMinimumSize(QSize(50, 50));
        pushButton_Close->setMaximumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/res/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Close->setIcon(icon2);
        pushButton_Close->setIconSize(QSize(50, 50));

        horizontalLayout_3->addWidget(pushButton_Close);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        label_UserName = new QLabel(frame);
        label_UserName->setObjectName(QString::fromUtf8("label_UserName"));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(20);
        label_UserName->setFont(font);

        horizontalLayout->addWidget(label_UserName);

        lineEdit_userName = new QLineEdit(frame);
        lineEdit_userName->setObjectName(QString::fromUtf8("lineEdit_userName"));
        lineEdit_userName->setMinimumSize(QSize(300, 40));
        lineEdit_userName->setMaximumSize(QSize(300, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(16);
        lineEdit_userName->setFont(font1);

        horizontalLayout->addWidget(lineEdit_userName);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_7);

        label_Password = new QLabel(frame);
        label_Password->setObjectName(QString::fromUtf8("label_Password"));
        label_Password->setFont(font);

        horizontalLayout_2->addWidget(label_Password);

        lineEdit_passWord = new QLineEdit(frame);
        lineEdit_passWord->setObjectName(QString::fromUtf8("lineEdit_passWord"));
        lineEdit_passWord->setMinimumSize(QSize(300, 40));
        lineEdit_passWord->setMaximumSize(QSize(300, 40));
        lineEdit_passWord->setFont(font1);

        horizontalLayout_2->addWidget(lineEdit_passWord);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_8);

        pushButton_Register = new QPushButton(frame);
        pushButton_Register->setObjectName(QString::fromUtf8("pushButton_Register"));
        pushButton_Register->setFont(font);

        horizontalLayout_4->addWidget(pushButton_Register);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton_Login = new QPushButton(frame);
        pushButton_Login->setObjectName(QString::fromUtf8("pushButton_Login"));
        pushButton_Login->setFont(font);

        horizontalLayout_4->addWidget(pushButton_Login);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_9);


        verticalLayout->addLayout(horizontalLayout_4);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        widget = new QWidget(frame);
        widget->setObjectName(QString::fromUtf8("widget"));
        horizontalLayout_6 = new QHBoxLayout(widget);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_10);

        pushButton_ConntecToServer = new QPushButton(widget);
        pushButton_ConntecToServer->setObjectName(QString::fromUtf8("pushButton_ConntecToServer"));
        pushButton_ConntecToServer->setFont(font);

        horizontalLayout_6->addWidget(pushButton_ConntecToServer);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_12);


        verticalLayout->addWidget(widget);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_IP = new QLabel(frame);
        label_IP->setObjectName(QString::fromUtf8("label_IP"));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bahnschrift Light"));
        font2.setPointSize(20);
        label_IP->setFont(font2);

        horizontalLayout_5->addWidget(label_IP);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        pushButton_Logout = new QPushButton(frame);
        pushButton_Logout->setObjectName(QString::fromUtf8("pushButton_Logout"));
        pushButton_Logout->setMinimumSize(QSize(93, 49));
        pushButton_Logout->setMaximumSize(QSize(93, 49));
        pushButton_Logout->setFont(font);

        horizontalLayout_5->addWidget(pushButton_Logout);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_11);

        label_Port = new QLabel(frame);
        label_Port->setObjectName(QString::fromUtf8("label_Port"));
        label_Port->setFont(font2);

        horizontalLayout_5->addWidget(label_Port);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(TcpClient);

        QMetaObject::connectSlotsByName(TcpClient);
    } // setupUi

    void retranslateUi(QWidget *TcpClient)
    {
        TcpClient->setWindowTitle(QCoreApplication::translate("TcpClient", "TcpClient", nullptr));
        pushButton_Settings->setText(QString());
        pushButton_Minwindow->setText(QString());
        pushButton_Close->setText(QString());
        label_UserName->setText(QCoreApplication::translate("TcpClient", "\350\264\246\345\217\267:", nullptr));
        label_Password->setText(QCoreApplication::translate("TcpClient", "\345\257\206\347\240\201:", nullptr));
        pushButton_Register->setText(QCoreApplication::translate("TcpClient", "\346\263\250\345\206\214", nullptr));
        pushButton_Login->setText(QCoreApplication::translate("TcpClient", "\347\231\273\345\275\225", nullptr));
        pushButton_ConntecToServer->setText(QCoreApplication::translate("TcpClient", "\350\277\236\346\216\245\346\234\215\345\212\241\345\231\250", nullptr));
        label_IP->setText(QCoreApplication::translate("TcpClient", "\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200", nullptr));
        pushButton_Logout->setText(QCoreApplication::translate("TcpClient", "\346\263\250\351\224\200", nullptr));
        label_Port->setText(QCoreApplication::translate("TcpClient", "\347\253\257\345\217\243\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TcpClient: public Ui_TcpClient {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TCPCLIENT_H

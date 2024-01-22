/********************************************************************************
** Form generated from reading UI file 'sharefile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREFILE_H
#define UI_SHAREFILE_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShareFile
{
public:
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_FriendList;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_Min;
    QPushButton *pushButton_Close;
    QScrollArea *scrollArea_ShowFriend;
    QWidget *scrollAreaWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_Confirm;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_Cancel;
    QSpacerItem *horizontalSpacer_4;

    void setupUi(QWidget *ShareFile)
    {
        if (ShareFile->objectName().isEmpty())
            ShareFile->setObjectName(QString::fromUtf8("ShareFile"));
        ShareFile->resize(542, 452);
        frame = new QFrame(ShareFile);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 541, 451));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_FriendList = new QLabel(frame);
        label_FriendList->setObjectName(QString::fromUtf8("label_FriendList"));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(14);
        label_FriendList->setFont(font);

        horizontalLayout->addWidget(label_FriendList);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton_Min = new QPushButton(frame);
        pushButton_Min->setObjectName(QString::fromUtf8("pushButton_Min"));
        pushButton_Min->setMinimumSize(QSize(0, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/minwindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Min->setIcon(icon);
        pushButton_Min->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Min);

        pushButton_Close = new QPushButton(frame);
        pushButton_Close->setObjectName(QString::fromUtf8("pushButton_Close"));
        pushButton_Close->setMinimumSize(QSize(0, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Close->setIcon(icon1);
        pushButton_Close->setIconSize(QSize(30, 30));

        horizontalLayout->addWidget(pushButton_Close);


        verticalLayout->addLayout(horizontalLayout);

        scrollArea_ShowFriend = new QScrollArea(frame);
        scrollArea_ShowFriend->setObjectName(QString::fromUtf8("scrollArea_ShowFriend"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(16);
        scrollArea_ShowFriend->setFont(font1);
        scrollArea_ShowFriend->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 525, 330));
        scrollArea_ShowFriend->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea_ShowFriend);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton_Confirm = new QPushButton(frame);
        pushButton_Confirm->setObjectName(QString::fromUtf8("pushButton_Confirm"));
        pushButton_Confirm->setMinimumSize(QSize(50, 50));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Bahnschrift"));
        font2.setPointSize(14);
        pushButton_Confirm->setFont(font2);

        horizontalLayout_2->addWidget(pushButton_Confirm);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButton_Cancel = new QPushButton(frame);
        pushButton_Cancel->setObjectName(QString::fromUtf8("pushButton_Cancel"));
        pushButton_Cancel->setMinimumSize(QSize(50, 50));
        pushButton_Cancel->setFont(font2);

        horizontalLayout_2->addWidget(pushButton_Cancel);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(ShareFile);

        QMetaObject::connectSlotsByName(ShareFile);
    } // setupUi

    void retranslateUi(QWidget *ShareFile)
    {
        ShareFile->setWindowTitle(QCoreApplication::translate("ShareFile", "Form", nullptr));
        label_FriendList->setText(QCoreApplication::translate("ShareFile", "\345\245\275\345\217\213\345\210\227\350\241\250", nullptr));
        pushButton_Min->setText(QString());
        pushButton_Close->setText(QString());
        pushButton_Confirm->setText(QCoreApplication::translate("ShareFile", "\347\241\256\345\256\232", nullptr));
        pushButton_Cancel->setText(QCoreApplication::translate("ShareFile", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShareFile: public Ui_ShareFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREFILE_H

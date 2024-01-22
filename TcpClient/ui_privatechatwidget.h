/********************************************************************************
** Form generated from reading UI file 'privatechatwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PRIVATECHATWIDGET_H
#define UI_PRIVATECHATWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PrivateChatWidget
{
public:
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_ShowFriendName;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_Min;
    QPushButton *pushButton_Close;
    QTextEdit *textEdit_ShowMessage;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_InputMessage;
    QPushButton *pushButton_SendMessage;

    void setupUi(QWidget *PrivateChatWidget)
    {
        if (PrivateChatWidget->objectName().isEmpty())
            PrivateChatWidget->setObjectName(QString::fromUtf8("PrivateChatWidget"));
        PrivateChatWidget->resize(692, 582);
        frame = new QFrame(PrivateChatWidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 0, 691, 581));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 5, 5, 5);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_ShowFriendName = new QLabel(frame);
        label_ShowFriendName->setObjectName(QString::fromUtf8("label_ShowFriendName"));
        QFont font;
        font.setFamily(QString::fromUtf8("Agency FB"));
        font.setPointSize(16);
        label_ShowFriendName->setFont(font);

        horizontalLayout_2->addWidget(label_ShowFriendName);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_Min = new QPushButton(frame);
        pushButton_Min->setObjectName(QString::fromUtf8("pushButton_Min"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/res/minwindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Min->setIcon(icon);
        pushButton_Min->setIconSize(QSize(30, 30));

        horizontalLayout_2->addWidget(pushButton_Min);

        pushButton_Close = new QPushButton(frame);
        pushButton_Close->setObjectName(QString::fromUtf8("pushButton_Close"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/res/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_Close->setIcon(icon1);
        pushButton_Close->setIconSize(QSize(30, 30));

        horizontalLayout_2->addWidget(pushButton_Close);


        verticalLayout->addLayout(horizontalLayout_2);

        textEdit_ShowMessage = new QTextEdit(frame);
        textEdit_ShowMessage->setObjectName(QString::fromUtf8("textEdit_ShowMessage"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Agency FB"));
        font1.setPointSize(20);
        textEdit_ShowMessage->setFont(font1);

        verticalLayout->addWidget(textEdit_ShowMessage);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        lineEdit_InputMessage = new QLineEdit(frame);
        lineEdit_InputMessage->setObjectName(QString::fromUtf8("lineEdit_InputMessage"));
        lineEdit_InputMessage->setFont(font);

        horizontalLayout->addWidget(lineEdit_InputMessage);

        pushButton_SendMessage = new QPushButton(frame);
        pushButton_SendMessage->setObjectName(QString::fromUtf8("pushButton_SendMessage"));
        pushButton_SendMessage->setMinimumSize(QSize(0, 38));

        horizontalLayout->addWidget(pushButton_SendMessage);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(PrivateChatWidget);

        QMetaObject::connectSlotsByName(PrivateChatWidget);
    } // setupUi

    void retranslateUi(QWidget *PrivateChatWidget)
    {
        PrivateChatWidget->setWindowTitle(QCoreApplication::translate("PrivateChatWidget", "Form", nullptr));
        label_ShowFriendName->setText(QString());
        pushButton_Min->setText(QString());
        pushButton_Close->setText(QString());
        textEdit_ShowMessage->setHtml(QCoreApplication::translate("PrivateChatWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Agency FB'; font-size:20pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:24pt;\"><br /></p></body></html>", nullptr));
        pushButton_SendMessage->setText(QCoreApplication::translate("PrivateChatWidget", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PrivateChatWidget: public Ui_PrivateChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PRIVATECHATWIDGET_H

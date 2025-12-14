/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QWidget *nen;
    QLabel *music_2;
    QPushButton *btnLogin;
    QPushButton *btnRegister;
    QWidget *avatar;

    void setupUi(QWidget *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName("login");
        login->resize(1317, 925);
        widget = new QWidget(login);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, -10, 1311, 951));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        nen = new QWidget(widget);
        nen->setObjectName("nen");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(nen->sizePolicy().hasHeightForWidth());
        nen->setSizePolicy(sizePolicy);
        nen->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        nen->setStyleSheet(QString::fromUtf8("/* V\303\255 d\341\273\245 1: M\303\240u xanh bi\341\273\203n nh\341\272\241t (Light Blue) */\n"
"background-color: #ADD8E6; \n"
"\n"
"/* V\303\255 d\341\273\245 2: M\303\240u xanh ng\341\273\215c lam (Turquoise) */\n"
"/* background-color: #40E0D0; */ \n"
"\n"
"/* V\303\255 d\341\273\245 3: M\303\240u v\303\240ng nh\341\272\241t (Light Yellow) */\n"
"/* background-color: #FFFFE0; */ \n"
"\n"
"/* V\303\255 d\341\273\245 4: M\303\240u cam nh\341\272\241t (Light Orange) */\n"
"/* background-color: #FFDAB9; */"));
        music_2 = new QLabel(nen);
        music_2->setObjectName("music_2");
        music_2->setGeometry(QRect(0, -5, 651, 91));
        QFont font;
        font.setPointSize(22);
        music_2->setFont(font);
        music_2->setStyleSheet(QString::fromUtf8("background-color: #A9D1C7; /* M\303\243 m\303\240u xanh l\341\273\245c nh\341\272\241t */\n"
"color: #1F1F1F; /* \304\220\341\272\267t m\303\240u ch\341\273\257 l\303\240 \304\221en \304\221\341\272\255m ho\341\272\267c x\303\241m \304\221\341\272\255m \304\221\341\273\203 d\341\273\205 \304\221\341\273\215c */"));
        music_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        btnLogin = new QPushButton(nen);
        btnLogin->setObjectName("btnLogin");
        btnLogin->setGeometry(QRect(170, 290, 221, 31));
        btnLogin->setStyleSheet(QString::fromUtf8("background-color: #A9D1C7; /* M\303\243 m\303\240u xanh l\341\273\245c nh\341\272\241t */\n"
"color: #1F1F1F; /* \304\220\341\272\267t m\303\240u ch\341\273\257 l\303\240 \304\221en \304\221\341\272\255m ho\341\272\267c x\303\241m \304\221\341\272\255m \304\221\341\273\203 d\341\273\205 \304\221\341\273\215c */"));
        btnRegister = new QPushButton(nen);
        btnRegister->setObjectName("btnRegister");
        btnRegister->setGeometry(QRect(170, 340, 221, 31));
        btnRegister->setStyleSheet(QString::fromUtf8("background-color: #A9D1C7; /* M\303\243 m\303\240u xanh l\341\273\245c nh\341\272\241t */\n"
"color: #1F1F1F; /* \304\220\341\272\267t m\303\240u ch\341\273\257 l\303\240 \304\221en \304\221\341\272\255m ho\341\272\267c x\303\241m \304\221\341\272\255m \304\221\341\273\203 d\341\273\205 \304\221\341\273\215c */"));

        horizontalLayout_2->addWidget(nen);

        avatar = new QWidget(widget);
        avatar->setObjectName("avatar");
        sizePolicy.setHeightForWidth(avatar->sizePolicy().hasHeightForWidth());
        avatar->setSizePolicy(sizePolicy);
        avatar->setStyleSheet(QString::fromUtf8("QWidget {\n"
"    background-image: url(\":/images/background.jpg\");\n"
"    background-position: center;\n"
"    background-repeat: no-repeat;\n"
"    background-origin: content;\n"
"    border-radius: 0px;\n"
"}\n"
"\n"
""));

        horizontalLayout_2->addWidget(avatar);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QWidget *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "Form", nullptr));
        music_2->setText(QCoreApplication::translate("login", "\360\237\216\265  MUSIC  \360\237\216\265", nullptr));
        btnLogin->setText(QCoreApplication::translate("login", "\304\220\304\203ng nh\341\272\255p", nullptr));
        btnRegister->setText(QCoreApplication::translate("login", "\304\220\304\203ng k\303\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H

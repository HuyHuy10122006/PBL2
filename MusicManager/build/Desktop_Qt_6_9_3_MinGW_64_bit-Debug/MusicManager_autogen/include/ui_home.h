/********************************************************************************
** Form generated from reading UI file 'home.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOME_H
#define UI_HOME_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Home
{
public:
    QWidget *widget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QFrame *frame;
    QWidget *widget_2;
    QHBoxLayout *horizontalLayout_2;
    QFrame *Sidebar;
    QPushButton *logout;
    QLabel *label;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QPushButton *home;
    QPushButton *playlist;
    QWidget *MainArea;
    QStackedWidget *stackedWidgetMain;
    QWidget *homePage;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *playlistPage;

    void setupUi(QWidget *Home)
    {
        if (Home->objectName().isEmpty())
            Home->setObjectName("Home");
        Home->resize(1280, 720);
        Home->setMinimumSize(QSize(1280, 720));
        Home->setMaximumSize(QSize(1280, 720));
        Home->setStyleSheet(QString::fromUtf8("QWidget { \n"
"    background-color: #121212; \n"
"    color: #E0E0E0; \n"
"    font-family: \"Segoe UI\"; \n"
"}\n"
""));
        widget = new QWidget(Home);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(0, 0, 1280, 80));
        widget->setMinimumSize(QSize(1280, 0));
        widget->setStyleSheet(QString::fromUtf8("background-color: #1F1F1F;\n"
"color: white;\n"
"border-radius: 8px;\n"
""));
        layoutWidget = new QWidget(widget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(370, 0, 902, 82));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(410);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit_2 = new QLineEdit(layoutWidget);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setMinimumSize(QSize(400, 0));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        font.setPointSize(11);
        lineEdit_2->setFont(font);
        lineEdit_2->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    background-color: #2A2A2A;\n"
"    color: white;\n"
"    border: none;\n"
"    border-radius: 15px;\n"
"    padding-left: 15px;\n"
"    height: 30px;\n"
"}\n"
"QLineEdit:focus {\n"
"    border: 1px solid #1DB954;\n"
"}\n"
""));

        horizontalLayout->addWidget(lineEdit_2);

        pushButton = new QPushButton(layoutWidget);
        pushButton->setObjectName("pushButton");
        pushButton->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setMinimumSize(QSize(50, 50));
        pushButton->setMaximumSize(QSize(50, 50));
        pushButton->setMouseTracking(false);
        pushButton->setTabletTracking(false);
        pushButton->setAcceptDrops(false);
        pushButton->setAutoFillBackground(false);
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    border-radius: 25px;   /* 50px / 2 = tr\303\262n */\n"
"    background-color: #666;\n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/avatar.jpg"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        pushButton->setIcon(icon);
        pushButton->setIconSize(QSize(80, 80));
        pushButton->setCheckable(false);
        pushButton->setAutoRepeat(false);
        pushButton->setAutoExclusive(false);
        pushButton->setFlat(true);

        horizontalLayout->addWidget(pushButton);

        frame = new QFrame(Home);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(0, 650, 1280, 67));
        frame->setMinimumSize(QSize(1280, 67));
        frame->setMaximumSize(QSize(1280, 65));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        widget_2 = new QWidget(Home);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(0, 80, 1280, 571));
        widget_2->setMinimumSize(QSize(1280, 571));
        widget_2->setMaximumSize(QSize(1280, 571));
        horizontalLayout_2 = new QHBoxLayout(widget_2);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        Sidebar = new QFrame(widget_2);
        Sidebar->setObjectName("Sidebar");
        Sidebar->setMaximumSize(QSize(170, 16777215));
        Sidebar->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent; \n"
"    color: #E0E0E0;\n"
"    border: none;\n"
"    font-size: 16px;\n"
"    text-align: left; \n"
"    padding: 10px 15px;\n"
"}\n"
"\n"
"QPushButton:checked {\n"
"    font-weight: bold;     \n"
"    color: #00BFA5;     \n"
"}\n"
""));
        Sidebar->setFrameShape(QFrame::Shape::StyledPanel);
        Sidebar->setFrameShadow(QFrame::Shadow::Raised);
        logout = new QPushButton(Sidebar);
        logout->setObjectName("logout");
        logout->setGeometry(QRect(0, 510, 170, 40));
        logout->setMinimumSize(QSize(170, 40));
        logout->setMaximumSize(QSize(170, 40));
        label = new QLabel(Sidebar);
        label->setObjectName("label");
        label->setGeometry(QRect(10, 10, 150, 121));
        label->setMinimumSize(QSize(150, 0));
        label->setMaximumSize(QSize(150, 16777215));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setItalic(false);
        label->setFont(font1);
        label->setStyleSheet(QString::fromUtf8("QLabel#labelLogo {\n"
"    color: #00BFA5;      \n"
"    font-weight: bold;     \n"
"    font-family: \"Segoe UI Semibold\"; \n"
"    font-size: 18pt;\n"
"    text-align: center;\n"
"}"));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        layoutWidget1 = new QWidget(Sidebar);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 200, 172, 121));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        home = new QPushButton(layoutWidget1);
        home->setObjectName("home");
        home->setMinimumSize(QSize(170, 40));
        home->setMaximumSize(QSize(170, 40));

        verticalLayout->addWidget(home);

        playlist = new QPushButton(layoutWidget1);
        playlist->setObjectName("playlist");
        playlist->setMinimumSize(QSize(170, 40));

        verticalLayout->addWidget(playlist);


        horizontalLayout_2->addWidget(Sidebar);

        MainArea = new QWidget(widget_2);
        MainArea->setObjectName("MainArea");
        stackedWidgetMain = new QStackedWidget(MainArea);
        stackedWidgetMain->setObjectName("stackedWidgetMain");
        stackedWidgetMain->setGeometry(QRect(0, 0, 1095, 555));
        sizePolicy.setHeightForWidth(stackedWidgetMain->sizePolicy().hasHeightForWidth());
        stackedWidgetMain->setSizePolicy(sizePolicy);
        stackedWidgetMain->setMinimumSize(QSize(1095, 555));
        stackedWidgetMain->setMaximumSize(QSize(16777215, 16777215));
        homePage = new QWidget();
        homePage->setObjectName("homePage");
        scrollArea = new QScrollArea(homePage);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setGeometry(QRect(-10, 0, 1101, 541));
        scrollArea->setMaximumSize(QSize(16777215, 541));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1085, 1600));
        scrollAreaWidgetContents->setMinimumSize(QSize(0, 1600));
        scrollArea->setWidget(scrollAreaWidgetContents);
        stackedWidgetMain->addWidget(homePage);
        playlistPage = new QWidget();
        playlistPage->setObjectName("playlistPage");
        stackedWidgetMain->addWidget(playlistPage);

        horizontalLayout_2->addWidget(MainArea);


        retranslateUi(Home);

        pushButton->setDefault(false);
        stackedWidgetMain->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Home);
    } // setupUi

    void retranslateUi(QWidget *Home)
    {
        Home->setWindowTitle(QCoreApplication::translate("Home", "Form", nullptr));
        lineEdit_2->setPlaceholderText(QCoreApplication::translate("Home", "Search ", nullptr));
        pushButton->setText(QString());
        logout->setText(QCoreApplication::translate("Home", "\360\237\224\222 \304\220\304\203ng xu\341\272\245t", nullptr));
        label->setText(QCoreApplication::translate("Home", "\360\237\216\265 MUSIC \360\237\216\265", nullptr));
        home->setText(QCoreApplication::translate("Home", "\360\237\217\240 Home", nullptr));
        playlist->setText(QCoreApplication::translate("Home", "\360\237\216\265 Playlist", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Home: public Ui_Home {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOME_H

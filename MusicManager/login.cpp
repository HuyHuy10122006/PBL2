#include "login.h"
#include "ui_login.h"
#include "AuthService.h"
#include "home.h"

#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QIcon>
#include <QMessageBox>

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    // Làm mờ ảnh bên phải (widget avatar)
    if(ui->avatar){
        QGraphicsBlurEffect *blur = new QGraphicsBlurEffect(this); // làm mờ
        blur->setBlurRadius(2.0);
        ui->avatar->setGraphicsEffect(blur);
    }

    // Overlay mờ toàn màn hình
    QLabel *overlay = new QLabel(this);
    overlay->setGeometry(this->rect());
    overlay->setStyleSheet("background-color: rgba(0,0,0,100);");
    overlay->hide();

    // Form trắng lớn
    QWidget *form = new QWidget(this);
    form->setFixedSize(350, 380);
    form->setStyleSheet("background-color: rgba(255,255,255,230); border-radius:10px;");
    form->hide();
    // Input fields
    QLineEdit *usernameEdit = new QLineEdit(form);
    usernameEdit->setGeometry(30, 100, 290, 30);
    usernameEdit->setStyleSheet("font-size:14px; color:black;");
    usernameEdit->setPlaceholderText("Nhập username...");

    QLineEdit *passwordEdit = new QLineEdit(form);
    passwordEdit->setGeometry(30, 160, 260, 30);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("font-size:14px; color:black;");
    passwordEdit->setPlaceholderText("Nhập password...");

    // Nút hiện/ẩn mật khẩu
    QPushButton *showPassBtn = new QPushButton(form);
    showPassBtn->setGeometry(295, 160, 35, 30);
    showPassBtn->setCheckable(true);

    QString eyePath = ":/images/eye.jpg";
    QString eyeSlashPath = ":/images/eye-slash.jpg";

    if(QFile::exists(eyePath) && QFile::exists(eyeSlashPath)){
        showPassBtn->setIcon(QIcon(eyePath));
        showPassBtn->setIconSize(QSize(20, 20));

        connect(showPassBtn, &QPushButton::toggled, this, [=](bool checked){
            if(checked){
                passwordEdit->setEchoMode(QLineEdit::Normal);
                showPassBtn->setIcon(QIcon(eyeSlashPath));
            } else {
                passwordEdit->setEchoMode(QLineEdit::Password);
                showPassBtn->setIcon(QIcon(eyePath));
            }
        });
    }

    // Nhập lại mật khẩu (dùng khi đăng ký)
    QLineEdit *rePasswordEdit = new QLineEdit(form);
    rePasswordEdit->setGeometry(30, 220, 260, 30);
    rePasswordEdit->setEchoMode(QLineEdit::Password);
    rePasswordEdit->setStyleSheet("font-size:14px; color:black;");
    rePasswordEdit->setPlaceholderText("Nhập lại password...");
    rePasswordEdit->hide();

    QPushButton *showRePassBtn = new QPushButton(form);
    showRePassBtn->setGeometry(295, 220, 35, 30);
    showRePassBtn->setCheckable(true);
    showRePassBtn->hide();

    if(QFile::exists(eyePath) && QFile::exists(eyeSlashPath)){
        showRePassBtn->setIcon(QIcon(eyePath));
        showRePassBtn->setIconSize(QSize(20, 20));

        connect(showRePassBtn, &QPushButton::toggled, this, [=](bool checked){
            if(checked){
                rePasswordEdit->setEchoMode(QLineEdit::Normal);
                showRePassBtn->setIcon(QIcon(eyeSlashPath));
            } else {
                rePasswordEdit->setEchoMode(QLineEdit::Password);
                showRePassBtn->setIcon(QIcon(eyePath));
            }
        });
    }

    // Labels
    QLabel *usernameLabel = new QLabel("Username", form);
    usernameLabel->setGeometry(30, 80, 100, 20);
    usernameLabel->setStyleSheet("color: gray; font-size:12px; font-weight: bold;");

    QLabel *passwordLabel = new QLabel("Password", form);
    passwordLabel->setGeometry(30, 140, 100, 20);
    passwordLabel->setStyleSheet("color: gray; font-size:12px; font-weight: bold;");

    QLabel *rePasswordLabel = new QLabel("Nhập lại Password", form);
    rePasswordLabel->setGeometry(30, 200, 150, 20);
    rePasswordLabel->setStyleSheet("color: gray; font-size:12px; font-weight: bold;");
    rePasswordLabel->hide();

    // Nút Submit & Quay lại
    QPushButton *submitBtn = new QPushButton("Đăng nhập", form);
    submitBtn->setGeometry(30, 270, 290, 40);
    submitBtn->setStyleSheet(
        "QPushButton { background-color: #2ecc71; color: white; font-size:16px; border-radius:8px; }"
        "QPushButton:hover { background-color: #27ae60; }"
        );
    connect(submitBtn, &QPushButton::clicked, this, [=](){
        //lấy dữ liệu user
        QString qUsername = usernameEdit->text();
        QString qPassword = passwordEdit->text();
        QString qRePassword = rePasswordEdit->text();
        //chuyển sang string
        std::string username = qUsername.toStdString();
        std::string password = qPassword.toStdString();
        std::string rePassword = qRePassword.toStdString();
        if(submitBtn->text() == "Đăng ký"){
            if(password != rePassword){
                QMessageBox::warning(this, "Đăng ký", "Mật khẩu nhập lại không khớp!");
                return;
            }
            if(AuthService::registerUser(username, password)){
                QMessageBox::information(this, "Đăng ký", "Đăng ký thành công!");
                form->hide();
                overlay->hide();
                this->hide();

                Home *homeWindow = new Home();
                homeWindow->show();
            } else{
                QMessageBox::warning(this, "Đăng ký", "Tài khoản đã tồn tại!");
            }
        }else{
            if(AuthService::loginUser(username, password)){
                QMessageBox::information(this, "Đăng nhập", "Đăng nhập thành công");
                form->hide();
                overlay->hide();
                this->hide();

                Home *homeWindow = new Home();
                homeWindow->show();
            } else{
                QMessageBox::warning(this, "Đăng nhập", "Sai usename hoặc password!");
            }
        }
    });

    QPushButton *backBtn = new QPushButton("Quay lại", form);
    backBtn->setGeometry(30, 320, 290, 40);
    backBtn->setStyleSheet(
        "QPushButton { background-color: #3498db; color: white; font-size:16px; border-radius:8px; }"
        "QPushButton:hover { background-color: #2980b9; }"
        );

    // Hiển thị form đăng nhập / đăng ký
    auto showForm = [=](const QString &type){
        ui->btnLogin->hide();
        ui->btnRegister->hide();
        overlay->show();
        overlay->raise();
        form->show();
        form->raise();
        form->move((this->width()-form->width())/2, (this->height()-form->height())/2);

        if(type == "Đăng nhập"){
            submitBtn->setText("Đăng nhập");
            rePasswordEdit->hide();
            rePasswordLabel->hide();
            showRePassBtn->hide();
        } else {
            submitBtn->setText("Đăng ký");
            rePasswordEdit->show();
            rePasswordLabel->show();
            showRePassBtn->show();
        }

        // Hiệu ứng mờ khi hiện form
        QGraphicsOpacityEffect *formEffect = new QGraphicsOpacityEffect(form);
        form->setGraphicsEffect(formEffect);
        QPropertyAnimation *anim = new QPropertyAnimation(formEffect, "opacity");
        anim->setDuration(400);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    };

    connect(ui->btnLogin, &QPushButton::clicked, this, [=](){ showForm("Đăng nhập"); });
    connect(ui->btnRegister, &QPushButton::clicked, this, [=](){ showForm("Đăng ký"); });

    // Nút Quay lại
    connect(backBtn, &QPushButton::clicked, this, [=](){
        form->hide();
        overlay->hide();
        ui->btnLogin->show();
        ui->btnRegister->show();
    });
}
login::~login()
{
    delete ui;
}

#include "home.h"
#include "ClickableLabel.h"
#include "ui_home.h"
#include "login.h"
#include <QMessageBox>
#include <qpushbutton.h>

// Home constructor
Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    // Mặc định hiển thị homePage
    ui->stackedWidgetMain->setCurrentWidget(ui->homePage);
    ui->home->setChecked(true);
    ui->playlist->setChecked(false);

    // Nút Home
    connect(ui->home, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->homePage);
        ui->home->setChecked(true);
        ui->playlist->setChecked(false);
    });

    // Nút Playlist
    connect(ui->playlist, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->playlistPage);
        ui->home->setChecked(false);
        ui->playlist->setChecked(true);
    });

    // Nút Đăng xuất
    connect(ui->logout, &QPushButton::clicked, this, [=](){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Đăng xuất",
                                      "Bạn có chắc chắn muốn đăng xuất không?",
                                      QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            this->close(); // đóng Home hiện tại
            login *loginForm = new login;
            loginForm->show(); // mở lại login
        }
    });

    // ========================
    // Click vào ca sĩ
    connect(ui->label_2, &ClickableLabel::clicked, this, [=](){
        showArtistDetail("Hai Thu Hiếu");
    });
    connect(ui->label_27, &ClickableLabel::clicked, this, [=](){
        showArtistDetail("B-Ray");
    });
    connect(ui->label_26, &ClickableLabel::clicked, this, [=](){
        showArtistDetail("LowG");
    });

    // Click vào mood
    connect(ui->anh14_2, &ClickableLabel::clicked, this, [=](){
        showMoodDetail("Happy");
    });
    connect(ui->anh24_2, &ClickableLabel::clicked, this, [=](){
        showMoodDetail("Sad");
    });

    // Nếu muốn: nút Home trên artistDetailPage hoặc moodDetailPage
    connect(ui->home, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->homePage);
        ui->home->setChecked(true);
        ui->playlist->setChecked(false);
    });
}

// ========================
// Slot hiển thị artistDetailPage
void Home::showArtistDetail(const QString &artistName)
{
    // Cập nhật tên ca sĩ
    ui->tencasy->setText(artistName); // QLabel trong artistDetailPage

    // Chuyển sang artistDetailPage
    ui->stackedWidgetMain->setCurrentWidget(ui->artistDetailPage);
}

// Slot hiển thị moodDetailPage
void Home::showMoodDetail(const QString &moodName)
{
    // Cập nhật tên mood
    ui->tamtrang->setText(moodName); // QLabel trong moodDetailPage

    // Chuyển sang moodDetailPage
    ui->stackedWidgetMain->setCurrentWidget(ui->moodDetailPage);
}

// Home destructor
Home::~Home()
{
    delete ui;
}

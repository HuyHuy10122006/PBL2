#include "home.h"
#include "ClickableLabel.h"
#include "ui_home.h"
#include "login.h"
#include "logic/musicManager.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QEvent>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QStyle> // Cần thiết để tính toán vị trí click trên Slider

// Home constructor
Home::Home(MusicManager *manager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home),
    m_manager(manager)
{
    ui->setupUi(this);

    // Mặc định hiển thị homePage
    ui->stackedWidgetMain->setCurrentWidget(ui->homePage);
    ui->home->setChecked(true);
    ui->playlist->setChecked(false);

    // Nạp dữ liệu các Section
    loadHomePageData();

    // --- CÁC KẾT NỐI CONNECT CỦA BẠN (GIỮ NGUYÊN) ---
    connect(ui->home, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->homePage);
        ui->home->setChecked(true);
        ui->playlist->setChecked(false);
    });

    connect(ui->playlist, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->playlistPage);
        ui->home->setChecked(false);
        ui->playlist->setChecked(true);
    });

    connect(ui->logout, &QPushButton::clicked, this, [=](){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Đăng xuất",
                                                                  "Bạn có chắc chắn muốn đăng xuất không?",
                                                                  QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            this->close();
            login *loginForm = new login(m_manager);
            loginForm->show();
        }
    });

    connect(ui->label_2, &ClickableLabel::clicked, this, [=](){ showArtistDetail("Hai Thu Hiếu"); });
    connect(ui->label_27, &ClickableLabel::clicked, this, [=](){ showArtistDetail("B-Ray"); });
    connect(ui->label_26, &ClickableLabel::clicked, this, [=](){ showArtistDetail("LowG"); });
    connect(ui->anh14_2, &ClickableLabel::clicked, this, [=](){ showMoodDetail("Happy"); });
    connect(ui->anh24_2, &ClickableLabel::clicked, this, [=](){ showMoodDetail("Sad"); });

    // --- HOÀN THIỆN LOGIC ĐIỀU KHIỂN (DÙNG ĐÚNG ID TỪ OBJECT INSPECTOR) ---

    if (m_manager && m_manager->getPlayer()) {
        QMediaPlayer* mediaPlayer = m_manager->getPlayer()->getMediaPlayer();

        // 1. Đồng bộ thanh kéo (sliderProgress)
        connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
            ui->sliderProgress->setRange(0, duration);
        });

        connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
            if (!ui->sliderProgress->isSliderDown()) {
                ui->sliderProgress->setValue(position);
            }
            int seconds = (position / 1000) % 60;
            int minutes = (position / 60000) % 60;
            // Cập nhật nhãn label_10
            ui->label_10->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
        });

        // 2. Nút Play/Pause (Dùng pushButton_21 từ image_9f4c26.png)
        connect(ui->pushButton_21, &QPushButton::clicked, this, [=](){
            if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
                mediaPlayer->pause();
            } else {
                mediaPlayer->play();
            }
        });

        // 3. Nút Next (pushButton_22) và Previous (pushButton_4)
        connect(ui->pushButton_22, &QPushButton::clicked, this, [=](){
            Playlist* cur = m_manager->getPlayer()->getAPlist();
            if (cur && cur->getName() != "INTERNAL_SYSTEM_PLAY") m_manager->next();
        });

        connect(ui->pushButton_4, &QPushButton::clicked, this, [=](){
            Playlist* cur = m_manager->getPlayer()->getAPlist();
            if (cur && cur->getName() != "INTERNAL_SYSTEM_PLAY") m_manager->previous();
        });

        // QUAN TRỌNG: Đăng ký Slider vào bộ lọc sự kiện để nhận diện cú click chuột
        ui->sliderProgress->installEventFilter(this);
    }
}

// Hàm nạp dữ liệu và xử lý "trong suốt" cho các widget con (GIỮ NGUYÊN)
void Home::setupSongUI(Song* s, QLabel* titleLbl, QLabel* artistLbl, QLabel* coverLbl, QFrame* songFrame) {
    if (!s || !songFrame) return;

    if (titleLbl) {
        titleLbl->setText(s->getTitle());
        titleLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    if (artistLbl) {
        artistLbl->setText(s->getArtist());
        artistLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    if (coverLbl) {
        QString imgPath = s->getCoverPath().isEmpty() ? ":/images/default_cover.jpg" : s->getCoverPath();
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            coverLbl->setPixmap(pix.scaled(coverLbl->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        }
        coverLbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    QList<QWidget*> children = songFrame->findChildren<QWidget*>();
    for (QWidget* child : children) {
        child->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    songFrame->setProperty("songPtr", QVariant::fromValue((void*)s));
    songFrame->installEventFilter(this);
    songFrame->setCursor(Qt::PointingHandCursor);
    songFrame->setStyleSheet("QFrame:hover { background-color: rgba(255, 255, 255, 25); border-radius: 10px; }");
}

bool Home::eventFilter(QObject *obj, QEvent *event) {
    // 1. XỬ LÝ THANH KÉO (Jump on click):
    if (obj == ui->sliderProgress && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // Tính toán giá trị dựa trên tọa độ pixel click chuột
            int newVal = QStyle::sliderValueFromPosition(
                ui->sliderProgress->minimum(),
                ui->sliderProgress->maximum(),
                mouseEvent->pos().x(),
                ui->sliderProgress->width()
                );
            ui->sliderProgress->setValue(newVal);
            m_manager->getPlayer()->getMediaPlayer()->setPosition(newVal);
            return true;
        }
    }

    // 2. XỬ LÝ CLICK KHUNG BÀI HÁT (GIỮ NGUYÊN)
    if (event->type() == QEvent::MouseButtonPress) {
        static bool isProcessing = false;
        if (isProcessing) return true;
        isProcessing = true;

        QFrame *frame = qobject_cast<QFrame*>(obj);
        if (frame && frame->property("songPtr").isValid()) {
            Song* s = static_cast<Song*>(frame->property("songPtr").value<void*>());
            if (s && m_manager) {
                m_manager->playSongByObject(s);
                // Cập nhật nhãn thông tin góc trái
                ui->SongTitle->setText(s->getTitle());
                ui->SongArtist->setText(s->getArtist());
                QString imgPath = s->getCoverPath().isEmpty() ? ":/images/default_cover.jpg" : s->getCoverPath();
                QPixmap pix(imgPath);
                if (!pix.isNull()) {
                    ui->labelSongImage->setPixmap(pix.scaled(ui->labelSongImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                }
                isProcessing = false;
                return true;
            }
        }
        isProcessing = false;
    }
    return QWidget::eventFilter(obj, event);
}

// --- CÁC HÀM CÒN LẠI GIỮ NGUYÊN ---
void Home::loadHomePageData() {
    if (!m_manager) return;
    DoubleLinkedList<Song*> sugSongs = m_manager->getRecommendedSongs(6);
    if (sugSongs.getSize() >= 1) setupSongUI(sugSongs(0), ui->label_14, ui->label_15, ui->label_13, ui->bai11);
    if (sugSongs.getSize() >= 2) setupSongUI(sugSongs(1), ui->label_8, ui->label_9, ui->label_7, ui->bai21);
    if (sugSongs.getSize() >= 3) setupSongUI(sugSongs(2), ui->label_5, ui->label_6, ui->label_3, ui->bai31);
    if (sugSongs.getSize() >= 4) setupSongUI(sugSongs(3), ui->label_20, ui->label_21, ui->label_19, ui->bai41);
    if (sugSongs.getSize() >= 5) setupSongUI(sugSongs(4), ui->label_16, ui->label_17, ui->label_4, ui->bai51);
    if (sugSongs.getSize() >= 6) setupSongUI(sugSongs(5), ui->label_22, ui->label_23, ui->label_18, ui->bai61);
}

void Home::showArtistDetail(const QString &artistName) {
    ui->tencasy->setText(artistName);
    ui->stackedWidgetMain->setCurrentWidget(ui->artistDetailPage);
}

void Home::showMoodDetail(const QString &moodName) {
    ui->tamtrang->setText(moodName);
    ui->stackedWidgetMain->setCurrentWidget(ui->moodDetailPage);
}

Home::~Home() { delete ui; }

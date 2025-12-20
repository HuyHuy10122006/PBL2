#include "home.h"
#include "ClickableLabel.h"
#include "ui_home.h"
#include "login.h"
#include "logic/musicManager.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QEvent>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QStyle>
#include <QMenu>
#include <QAction>

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
    setupPlayerControls();
    loadPlaylistPage();
    connect(ui->themplaylist, &QPushButton::clicked, this, [=](){
        bool ok;
        QString name = QInputDialog::getText(this, "Tạo Playlist", "Nhập tên Playlist mới:",
                                             QLineEdit::Normal, "", &ok);
        if (ok && !name.isEmpty()) {
            if (m_manager->getPlaylist(name) == nullptr) {
                Playlist* newPl = new Playlist(name);
                m_manager->addPlaylist(newPl);
                m_manager->savePlaylists();
                loadPlaylistPage(); // Vẽ lại giao diện ngay lập tức
            } else {
                QMessageBox::warning(this, "Lỗi", "Playlist này đã tồn tại!");
            }
        }
    });

    // Kết nối nút Playlist ở menu trái để luôn nạp mới khi nhấn vào
    connect(ui->playlist, &QPushButton::clicked, this, [=](){
        ui->stackedWidgetMain->setCurrentWidget(ui->playlistPage);
        ui->home->setChecked(false);
        ui->playlist->setChecked(true);
        loadPlaylistPage();
    });

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
    for (QWidget* &child : children) {
        child->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    songFrame->setProperty("songPtr", QVariant::fromValue((void*)s));
    songFrame->installEventFilter(this);
    songFrame->setCursor(Qt::PointingHandCursor);
    songFrame->setStyleSheet("QFrame:hover { background-color: rgba(255, 255, 255, 25); border-radius: 10px; }");
    // 1. Cho phép Frame nhận sự kiện chuột phải
    songFrame->setContextMenuPolicy(Qt::CustomContextMenu);

    // 2. Kết nối tín hiệu chuột phải
    connect(songFrame, &QFrame::customContextMenuRequested, this, [=](const QPoint &pos) {
        QMenu contextMenu(this);
        contextMenu.setStyleSheet("QMenu { background-color: #2A2A2A; color: white; border: 1px solid #444; }"
                                  "QMenu::item:selected { background-color: #3A3A3A; }");

        QMenu *addMenu = contextMenu.addMenu("Thêm vào Playlist...");

        // Lấy danh sách playlist từ m_manager
        DoubleLinkedList<Playlist*>& allPls = m_manager->getPlaylists();
        bool hasPlaylist = false;

        for (int i = 0; i < allPls.getSize(); ++i) {
            Playlist* pl = allPls(i);
            if (pl->getIsTemporary()) continue;

            hasPlaylist = true;
            QAction *action = addMenu->addAction(pl->getName());

            // Khi chọn một playlist để thêm bài hát
            connect(action, &QAction::triggered, this, [=]() {
                m_manager->addSongToPlaylist(pl->getName(), s);
                QMessageBox::information(this, "Thành công",
                                          QString("Đã thêm '%1' vào playlist '%2'").arg(s->getTitle(), pl->getName()));
            });
        }

        if (!hasPlaylist) {
            addMenu->addAction("Chưa có playlist nào")->setEnabled(false);
        }

        // Hiển thị menu tại đúng vị trí chuột
        contextMenu.exec(songFrame->mapToGlobal(pos));
    });
}

bool Home::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame *frame = qobject_cast<QFrame*>(obj);
        // Nếu khung đó chứa thông tin Playlist (đã gán ở loadPlaylistPage)
        if (frame && frame->property("playlistPtr").isValid()) {
            Playlist* pl = static_cast<Playlist*>(frame->property("playlistPtr").value<void*>());
            if (pl) {
                hienThiChiTietPlaylist(pl); // Gọi hàm hiển thị bài hát bên dưới
                return true;
            }
        }
    }
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
void Home::setupPlayerControls() {
    if (!m_manager || !m_manager->getPlayer()) return;

    QMediaPlayer* mediaPlayer = m_manager->getPlayer()->getMediaPlayer();
    QAudioOutput* audioOutput = m_manager->getPlayer()->getAudioOutput();

    connect(ui->pushButton_21, &QPushButton::clicked, this, [=](){
        qDebug() << "Nut Play da duoc bam!";
        if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            mediaPlayer->pause();
            ui->pushButton_21->setText("▶");
        } else {
            mediaPlayer->play();
            ui->pushButton_21->setText("⏸");
        }
    });


    // 2. THANH TIẾN TRÌNH (ID: sliderProgress)
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [=](qint64 duration) {
        ui->sliderProgress->setRange(0, duration);
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position) {
        if (!ui->sliderProgress->isSliderDown()) {
            ui->sliderProgress->setValue(position);
        }
        // Cập nhật nhãn thời gian hiện tại (ID: label_10)
        int seconds = (position / 1000) % 60;
        int minutes = (position / 60000) % 60;
        ui->label_10->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));
    });

    // Tua nhạc khi kéo slider
    connect(ui->sliderProgress, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    // 3. THANH ÂM LƯỢNG (ID: horizontalSlider)
    ui->horizontalSlider->setRange(0, 100);
    ui->horizontalSlider->setValue(70);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, [=](int value) {
        if (audioOutput) {
            audioOutput->setVolume(value / 100.0);
        }
    });
    // 5. EVENT FILTER (Cho phép click nhảy vị trí trên slider)
    ui->sliderProgress->installEventFilter(this);
    // Tự động cập nhật giao diện khi bài hát thay đổi (Source Changed)
    // Sửa đoạn connect(mediaPlayer, &QMediaPlayer::sourceChanged, ...) tại dòng 195
    connect(mediaPlayer, &QMediaPlayer::sourceChanged, this, [=](){
        if (m_manager->getPlayer()) {
            Playlist* ap = m_manager->getPlayer()->getAPlist();
            // Lấy thông tin bài hát đang phát từ MusicPlayer thông qua currentIndex
            // Giả sử bạn đã có getter getCurrentIndex() trong MusicPlayer
            // Hoặc đơn giản là lấy thông tin từ UI nếu m_manager->playSong đã cập nhật nó.
            ui->pushButton_21->setText("⏸");
        }
    });
    connect(ui->pushButton_21, &QPushButton::clicked, this, [=](){
        qDebug() << "Nut Play da duoc bam!"; // Neu bam ma khong hien dong nay, thi connect dang loi
        if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
            mediaPlayer->pause();
            ui->pushButton_21->setText("▶");
        } else {
            mediaPlayer->play();
            ui->pushButton_21->setText("⏸");
        }
    });
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this,
            [=](qint64 duration) {
                qDebug() << "Duration =" << duration;
                ui->sliderProgress->setRange(0, duration);
            });
}
void Home::loadPlaylistPage() {
    if (!m_manager) return;

    // 1. Dọn dẹp layout cũ trong verticalLayout_18
    QLayoutItem *child;
    while ((child = ui->verticalLayout_18->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }

    // 2. Lấy danh sách từ Manager
    DoubleLinkedList<Playlist*>& allPlaylists = m_manager->getPlaylists();

    for (int i = 0; i < allPlaylists.getSize(); ++i) {
        Playlist* pl = allPlaylists(i);
        if (pl->getIsTemporary()) continue;

        QFrame* plFrame = new QFrame(this);
        plFrame->setMinimumHeight(70);
        plFrame->setCursor(Qt::PointingHandCursor);
        plFrame->setStyleSheet("QFrame { background-color: #2A2A2A; border-radius: 12px; margin-bottom: 5px; }"
                               "QFrame:hover { background-color: #3A007B; }");

        // GÁN CON TRỎ PLAYLIST VÀO FRAME
        plFrame->setProperty("playlistPtr", QVariant::fromValue((void*)pl));
        plFrame->installEventFilter(this); // Cho phép nhận sự kiện click

        QHBoxLayout* layout = new QHBoxLayout(plFrame);
        QLabel* name = new QLabel(pl->getName(), plFrame);
        name->setStyleSheet("color: white; font-size: 13pt; font-weight: bold;");
        name->setAttribute(Qt::WA_TransparentForMouseEvents); // Để click xuyên qua label vào frame

        layout->addWidget(name);
        ui->verticalLayout_18->addWidget(plFrame);
    }
    ui->verticalLayout_18->addStretch();
}
void Home::hienThiChiTietPlaylist(Playlist* pl) {
    if (!pl) return;

    // 1. Chuyển sang trang chi tiết (widget_17 chứa trang detail của bạn)
    ui->stackedWidgetMain->setCurrentWidget(ui->playlistDetailPage);

    // 2. Cập nhật tên playlist lên Label (ID từ XML của bạn là "tenp")
    ui->tenp->setText(pl->getName());

    // 3. Thiết lập Layout cho vùng hiển thị bài hát (widget_31 trong XML)
    if (!ui->widget_31->layout()) {
        QVBoxLayout* layout = new QVBoxLayout(ui->widget_31);
        layout->setContentsMargins(10, 10, 10, 10);
        layout->setSpacing(5);
    }

    // 4. Dọn dẹp danh sách bài hát cũ (tránh bị hiện trùng bài của playlist trước)
    QLayoutItem *child;
    while ((child = ui->widget_31->layout()->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        }
        delete child;
    }

    // 5. Lấy danh sách bài hát thực tế từ đối tượng Playlist
    const DoubleLinkedList<Song*>& songs = pl->getSongs();

    if (songs.isEmpty()) {
        QLabel* empty = new QLabel("Playlist này chưa có bài hát nào", ui->widget_31);
        empty->setStyleSheet("color: gray; font-style: italic; font-size: 12pt;");
        ui->widget_31->layout()->addWidget(empty);
    } else {
        for (int i = 0; i < songs.getSize(); ++i) {
            Song* s = songs(i);

            // Tạo một khung nhỏ cho mỗi bài hát
            QFrame* sFrame = new QFrame(ui->widget_31);
            sFrame->setMinimumHeight(50);
            sFrame->setStyleSheet("QFrame { background-color: #1E1E1E; border-radius: 5px; }"
                                  "QFrame:hover { background-color: #2A2A2A; }");

            QHBoxLayout* sLayout = new QHBoxLayout(sFrame);

            QLabel* title = new QLabel(s->getTitle(), sFrame);
            title->setStyleSheet("color: white; font-weight: bold;");

            QLabel* artist = new QLabel(s->getArtist(), sFrame);
            artist->setStyleSheet("color: #AAAAAA;");

            sLayout->addWidget(title);
            sLayout->addSpacing(20);
            sLayout->addWidget(artist);
            sLayout->addStretch();

            // QUAN TRỌNG: Gán songPtr để nhấn vào là phát nhạc ngay
            sFrame->setProperty("songPtr", QVariant::fromValue((void*)s));
            sFrame->installEventFilter(this);
            sFrame->setCursor(Qt::PointingHandCursor);

            sFrame->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(sFrame, &QFrame::customContextMenuRequested, this, [=](const QPoint &pos) {
                QMenu contextMenu(this);
                // Bạn có thể chỉnh style cho Menu giống các phần trước
                contextMenu.setStyleSheet("QMenu { background-color: #2A2A2A; color: white; border: 1px solid #444; }"
                                          "QMenu::item:selected { background-color: #3A3A3A; }");

                QAction *removeAction = contextMenu.addAction("Xóa khỏi Playlist này");

                connect(removeAction, &QAction::triggered, this, [=]() {
                    QMessageBox::StandardButton reply = QMessageBox::question(this, "Xác nhận xóa",
                                                                              QString("Bạn có muốn xóa bài hát '%1' khỏi playlist này không?").arg(s->getTitle()),
                                                                              QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::Yes) {
                        // Gọi hàm xóa đã viết trong MusicManager
                        m_manager->removeSongFromPlaylist(pl->getName(), s->getTitle(), s->getArtist());

                        // QUAN TRỌNG: Gọi lại chính hàm này để làm mới (refresh) giao diện ngay lập tức
                        hienThiChiTietPlaylist(pl);
                    }
                });

                contextMenu.exec(sFrame->mapToGlobal(pos));
            });
            ui->widget_31->layout()->addWidget(sFrame);
        }
    }
    // Thêm khoảng trống ở cuối để đẩy các bài hát lên trên
    static_cast<QVBoxLayout*>(ui->widget_31->layout())->addStretch();
}

Home::~Home() { delete ui; }

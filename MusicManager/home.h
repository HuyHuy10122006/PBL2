#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QFrame>
#include "logic/musicManager.h" //

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    // BẮT BUỘC: Constructor phải có tham số MusicManager*
    explicit Home(MusicManager *manager, QWidget *parent = nullptr);
    ~Home();

protected:
    // BẮT BUỘC: Để bắt sự kiện click vào khung bài hát
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void showArtistDetail(const QString &artistName);
    void showMoodDetail(const QString &moodName);
    void loadHomePageData(); //

private:
    void setupSongUI(Song* s, QLabel* titleLbl, QLabel* artistLbl, QLabel* coverLbl, QFrame* songFrame); //

    Ui::Home *ui;
    MusicManager *m_manager; //
    void setupPlayerControls();
    void loadPlaylistPage();
    void hienThiChiTietPlaylist(Playlist* pl);
};

#endif

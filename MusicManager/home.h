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

    explicit Home(MusicManager *manager, QWidget *parent = nullptr);
    ~Home();

protected:

    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void showArtistDetail(const QString &artistName);
    void showMoodDetail(const QString &moodName);
    void loadHomePageData(); //

private:
    int m_lastVolume = 70;
    bool m_isMuted = false;
    void setupSongUI(Song* s, QLabel* titleLbl, QLabel* artistLbl, QLabel* coverLbl, QFrame* songFrame); //

    Ui::Home *ui;
    MusicManager *m_manager;
    QString m_currentPlaylistSearch = "";
    void setupPlayerControls();
    void loadPlaylistPage();
    void hienThiChiTietPlaylist(Playlist* pl);
};

#endif

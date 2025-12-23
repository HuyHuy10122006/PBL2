#include "musicplayer.h"
#include <QUrl>
#include <QDebug>

MusicPlayer::MusicPlayer()
    : APlist(nullptr), currentIndex(0)
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);  
}

MusicPlayer::~MusicPlayer() {
    player->stop();
    delete player;
    delete audioOutput;
}
Song* MusicPlayer::getCurrentSong() {
    if (APlist && currentIndex >= 0 && currentIndex < APlist->getSongs().getSize()) {
        return APlist->getSongs()(currentIndex); // Trả về bài hát tại vị trí hiện tại
    }
    return nullptr;
}

QMediaPlayer* MusicPlayer::getMediaPlayer() {
    return player;
}

void MusicPlayer::addPlist(Playlist* Plist)
{
    if(!Plist) return;
    
    // Kiểm tra trùng lặp bằng con trỏ thay vì giá trị để tránh crash vùng nhớ
    for(int i = 0; i < playlists.getSize(); i++)
    {
        if(playlists(i) == Plist) return; 
    }
    
    playlists.append(Plist);
    if(!APlist)
    {
        APlist = Plist;
        currentIndex = 0;
    }
}

void MusicPlayer::setAPlist(const QString& name)
{
    for(int i = 0; i < playlists.getSize(); i++)
    {
        // Kiểm tra con trỏ hợp lệ trước khi truy cập getName()
        if(playlists(i) && playlists(i)->getName() == name)
        {
            APlist = playlists(i);
            return;
        }
    }
}
 
Playlist* MusicPlayer::getAPlist() const {
    return APlist;
}
void MusicPlayer::setSource(const QUrl &source) {
    if (player) {
        player->setSource(source);
    }
}
void MusicPlayer::play(int index) {
    if(!APlist) return;
    
    // Đảm bảo lấy danh sách bài hát an toàn
    auto songs = APlist->getSongs();
    if(index < 0 || index >= songs.getSize()) return;

    Song* currentSong = songs(index);
    if (!currentSong) return; // Kiểm tra con trỏ Song

    // 1. DỪNG PHÁT VÀ GIẢI PHÓNG BỘ NHỚ FILE CŨ
    player->stop(); 
    player->setSource(QUrl()); // Buông file cũ ra để tránh lỗi Access Violation

    currentIndex = index;
    
    // Cập nhật PlayCount
    currentSong->setPlayCount(currentSong->getPlayCount() + 1);

    qDebug() << "Dang co gang mo file:" << currentSong->getFilePath();
    
    // 2. THIẾT LẬP NGUỒN MỚI VÀ PHÁT
    player->setSource(QUrl::fromLocalFile(currentSong->getFilePath()));
    player->play();
}

void MusicPlayer::stop() {
    if(player) player->stop();
}

void MusicPlayer::next() {
    if(!APlist || APlist->getSongs().isEmpty()) return; 
    
    int totalSongs = APlist->getSongs().getSize();
    if (totalSongs <= 0) return;

    currentIndex = (currentIndex + 1) % totalSongs;
    play(currentIndex);
}

void MusicPlayer::previous() {
    if(!APlist || APlist->getSongs().isEmpty()) return;
    int size = APlist->getSongs().getSize();
    currentIndex = (currentIndex - 1 + size) % size;
    play(currentIndex);
}

void MusicPlayer::pause() {
    if(player) player->pause();
}
#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include "playlist.h"
#include "doublelinkedlist.h"
class MusicPlayer{
private:
    QMediaPlayer *player;                 // Đối tượng phát nhạc chính
    QAudioOutput *audioOutput;            // Đối tượng điều khiển âm thanh (volume/output)
    DoubleLinkedList<Playlist*> playlists; // Danh sách các playlist
    Playlist *APlist;                     // Playlist đang được chọn (Active Playlist)
    int currentIndex;                     // Vị trí bài hát hiện tại trong playlist

public:
    MusicPlayer();    
    MusicPlayer(Playlist* Plist); // Constructor với playlist khởi tạo               
    ~MusicPlayer();                       

    void addPlist(Playlist* Plist);       // Thêm playlist vào danh sách
    void removePlist(const QString& name); // Xóa playlist theo tên
    void setAPlist(const QString& name);   // Đặt playlist hiện tại theo tên
    Playlist* getAPlist() const;           // Lấy playlist hiện tại
    
    void play(int index);                 // Phát bài hát theo index trong playlist hiện tại
    void playSingleSong(const QString& songTitle, const QString& artistName); // Phát 1 bài lẻ, không cần playlist
    void stop();                          // Dừng phát nhạc
    void next();                          // Chuyển sang bài kế tiếp
    void previous();                      // Quay lại bài trước
    void pause();                         // Tạm dừng phát
};

#endif

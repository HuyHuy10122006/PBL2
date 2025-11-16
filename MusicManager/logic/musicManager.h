#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H
#include <QString>
#include <QDebug>
#include <stdexcept>
#include "playlist.h"
#include "musicplayer.h"
#include "doublelinkedlist.h"
class MusicManager{
private:
    DoubleLinkedList<Playlist*> playlists;   // danh sách tất cả playlist
    DoubleLinkedList<Song*> songsOnHome;      // danh sách tất cả bài hát hiển thị ngoài Home
    MusicPlayer* player;                      // bộ phát nhạc (play/next/prev)
public:
    MusicManager();                          
    ~MusicManager();                          
    void addPlaylist(Playlist* playlist);      // thêm playlist mới
    void removePlaylist(const QString& name);  // xóa playlist theo tên
    Playlist* getPlaylist(const QString& name) const;   // lấy playlist theo tên
    DoubleLinkedList<Playlist*>& getPlaylists();        // trả danh sách playlist
    void addSongToPlaylist(const QString& playlistName, Song* song); 
    void removeSongFromPlaylist(const QString& playlistName,
                                const QString& songTitle,
                                const QString& artistName);
    DoubleLinkedList<Song*> searchHomeSong(const QString& word) const;  // tìm kiếm bài hát trên Home
    void playSong(const QString& playlistName, int index); // phát 1 bài theo index trong playlist
    void stop();        // dừng phát nhạc
    void next();        // phát bài tiếp theo
    void previous();    // phát bài trước đó
    void pause();       // tạm dừng
    void listAllPlaylists() const;   // in playlist
};
#endif

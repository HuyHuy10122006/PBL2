#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QString>
#include <QDebug>
#include "song.h"
#include "doublelinkedlist.h"
class Playlist{
private:
    QString name;
    DoubleLinkedList<Song*> songs; 
public:
    Playlist(const QString &name = "Default"); // Constructor
    bool operator ==(const Playlist &p) const; // So sánh hai Playlist (theo tên)
    QString getName() const; // Lấy tên Playlist  
    void addSong(Song* song); // Thêm bài hát vào cuối Playlist
    void removeSong(const QString &title, const QString &artist); // Xóa bài hát theo Tiêu đề/Nghệ sĩ
    void listSongs() const; // In danh sách bài hát ra cửa sổ debug
    int totalDuration() const; // Tính tổng thời lượng Playlist (giây)
    bool isEmpty() const; // Kiểm tra Playlist có trống không
    const DoubleLinkedList<Song*>& getSongs() const; // Lấy danh sách bài hát
    DoubleLinkedList<Song*> searchSongs(const QString &in) const; // Tìm kiếm bài hát theo chuỗi nhập vào
    ~Playlist(); // Destructor
};
#endif
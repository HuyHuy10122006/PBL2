#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H
#include <QString>
#include <QDebug>
#include <stdexcept>
#include "playlist.h"
#include "musicplayer.h"
#include "doublelinkedlist.h"
#include "song.h"

class MusicManager{
private:
    DoubleLinkedList<Playlist*> playlists; 	// Danh sách tất cả các playlist 
    DoubleLinkedList<Song*> allSongs; 	 //catalog 	
    DoubleLinkedList<Song*> songsOnHome; 	 // Danh sách hiển thị trên Home
    MusicPlayer* player; 	
public:
    MusicManager(); 
    ~MusicManager();
    void addSongToCatalog(Song* song); 	// Thêm bài hát vào catalog 
    void addSongToHome(Song* song);    	// Thêm bài hát vào danh sách hiển thị Home 
    const DoubleLinkedList<Song*>& getSongsOnHome() const { return songsOnHome; } // Lấy danh sách các bài hát trên Home
    MusicPlayer* getPlayer() const; // Lấy con trỏ đến MusicPlayer

    void addPlaylist(Playlist* playlist); 
    void removePlaylist(const QString& name);
    Playlist* getPlaylist(const QString& name) const;// Lấy playlist theo tên
    DoubleLinkedList<Playlist*>& getPlaylists(); // Lấy danh sách tất cả playlist
    
    void addSongToPlaylist(const QString& playlistName, Song* song);  // Thêm bài hát vào playlist theo tên
    void removeSongFromPlaylist(const QString& playlistName,const QString& songTitle,const QString& artistName);// Xóa bài hát khỏi playlist 
                                
    DoubleLinkedList<Song*> searchHomeSong(const QString& word) const; // Tìm kiếm bài hát trên Home theo từ khóa
    DoubleLinkedList<Playlist*> searchPlaylist(const QString& word) const;// Tìm kiếm playlist theo từ khóa

    void playSong(const QString& playlistName, int index); // Phát bài hát trong playlist
    void playSingleSong(const QString& songTitle, const QString& artistName); // Phát bài hát đơn
    void stop(); 		// Dừng phát
    void next(); 		// Phát bài hát tiếp theo
    void previous(); 	// Phát bài hát trước đó
    void pause(); 		// Tạm dừng phát
    void listAllPlaylists() const;  // In danh sách tất cả playlist 
};

#endif 
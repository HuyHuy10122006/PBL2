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
    DoubleLinkedList<Playlist*> playlists;// Danh sach tat ca cac playlist
    DoubleLinkedList<Song*> allSongs;// Catalog
    DoubleLinkedList<Song*> songsOnHome;// Danh sach hien thi tren Home
    MusicPlayer* player; 
public:
    MusicManager(); 
    ~MusicManager();
    void addSongToCatalog(Song* song);// Them bai hat vao catalog
    void addSongToHome(Song* song);// Them bai hat vao danh sach hien thi Home
    const DoubleLinkedList<Song*>& getSongsOnHome() const { return songsOnHome; }// Lay danh sach cac bai hat tren Home
    MusicPlayer* getPlayer() const;// Lay con tro den MusicPlayer

    void addPlaylist(Playlist* playlist); 
    void removePlaylist(const QString& name);
    Playlist* getPlaylist(const QString& name) const;// Lay playlist theo ten
    DoubleLinkedList<Playlist*>& getPlaylists();// Lay danh sach tat ca playlist
    
    void addSongToPlaylist(const QString& playlistName, Song* song);// Them bai hat vao playlist theo ten
    void removeSongFromPlaylist(const QString& playlistName,const QString& songTitle,const QString& artistName);// Xoa bai hat khoi playlist 
    DoubleLinkedList<Song*> searchHomeSong(const QString& word) const;// Tim kiem bai hat tren Home theo tu khoa
    DoubleLinkedList<Playlist*> searchPlaylist(const QString& word) const;// Tim kiem playlist theo tu khoa

    void playSong(const QString& playlistName, int index);// Phat bai hat trong playlist
    void playSingleSong(const QString& songTitle, const QString& artistName);// Phat bai hat don
    void stop();// Dung phat
    void next();// Phat bai hat tiep theo
    void previous();// Phat bai hat truoc do
    void pause();// Tam dung phat
    void listAllPlaylists() const;// In danh sach tat ca playlist

   //home
    DoubleLinkedList<Song*> getTopSongs(int count) const;
    DoubleLinkedList<Song*> getNewReleases(int count) const;
    DoubleLinkedList<Song*> getSongsByArtist(const QString& artistName) const;
    DoubleLinkedList<Song*> getRecommendedSongs(int count) const;

    // luu va tai du lieu
    void saveData() const;
    void loadData(); 
};

#endif

#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H
#include <QString>
#include <QDebug>
#include "playlist.h"
#include "musicplayer.h"
#include "doublelinkedlist.h"
#include "song.h"

class MusicManager{
private:
    DoubleLinkedList<Playlist*> playlists;
    DoubleLinkedList<Song*> allSongs;
    DoubleLinkedList<Song*> songsOnHome;
    MusicPlayer* player; 
public:
    MusicManager(); 
    ~MusicManager();
    void addSongToCatalog(Song* song);
    void addSongToHome(Song* song);
    const DoubleLinkedList<Song*>& getAllSongs() const { return allSongs; }
    const DoubleLinkedList<Song*>& getSongsOnHome() const { return songsOnHome; }
    MusicPlayer* getPlayer() const;

    void addPlaylist(Playlist* playlist); 
    void removePlaylist(const QString& name);
    void renamePlaylist(const QString& oldName, const QString& newName);
    Playlist* getPlaylist(const QString& name) const;
    DoubleLinkedList<Playlist*>& getPlaylists();
    void savePlaylists() const;
    void loadPlaylists();
    
    void addSongToPlaylist(const QString& playlistName, Song* song);
    void removeSongFromPlaylist(const QString& playlistName,const QString& songTitle,const QString& artistName);
    DoubleLinkedList<Song*> searchHomeSong(const QString& word) const;
    DoubleLinkedList<Playlist*> searchPlaylist(const QString& word) const;

    void playSong(const QString& playlistName, int index);
    void playSingleSong(const QString& songTitle, const QString& artistName);
    void playSongByObject(Song* s); // [MỚI] Phát trực tiếp từ đối tượng Song
    void stop();
    void next();
    void previous();
    void pause();
    void listAllPlaylists() const;

    DoubleLinkedList<Song*> getTopSongs(int count) const;
    DoubleLinkedList<Song*> getNewReleases(int count) const;
    DoubleLinkedList<Song*> getSongsByArtist(const QString& artistName) const;
    DoubleLinkedList<Song*> getRecommendedSongs(int count) const;
    DoubleLinkedList<Song*> getSongsByMood(const QString& moodName) const;

    void saveData() const;
    void loadData(); 
};
#endif
#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QString>
#include <QDebug>
#include <stdexcept>
#include "playlist.h"
#include "musicplayer.h"
#include "doublelinkedlist.h"

class MusicManager{
    DoubleLinkedList<Playlist*> playlists;
    MusicPlayer* player;   
    public:
    MusicManager();
    ~MusicManager();

    void addPlaylist(Playlist* playlist);
    void removePlaylist(const QString& name);
    Playlist* getPlaylist(const QString& name) const;
    DoubleLinkedList<Playlist*>& getPlaylists();

    void addSongToPlaylist(const QString& playlistName, Song* song);
    void removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName);
    DoubleLinkedList<Song*> searchHomeSong(const QString& word) const;

    void playSong(const QString& playlistName, int index);
    void stop();
    void next();
    void previous();
    void pause();

    void listAllPlaylists() const;
};

#endif
#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <QString>
#include <QDebug>
#include "song.h"
#include "doublelinkedlist.h"
class Playlist{
private:
    QString name;
    DoubleLinkedList<Song> songs;
public:
    Playlist(const QString &name = "Default");
    void addSong(const Song &song);
    void removeSong(const QString &title);
    void listSongs() const;
    int totalDuration() const;
    const DoubleLinkedList<Song>& getSongs() const; 
};
#endif

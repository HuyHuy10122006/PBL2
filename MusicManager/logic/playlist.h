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
    bool isTemporary; 

public:
    Playlist(const QString &name = "Default", bool isTemporary = false); 

    bool getIsTemporary() const; 

    bool operator ==(const Playlist &p) const; 
    QString getName() const; 
    void addSong(Song* song); 
    void removeSong(const QString &title, const QString &artist); 
    void listSongs() const; 
    int totalDuration() const; 
    bool isEmpty() const; 
    const DoubleLinkedList<Song*>& getSongs() const; 
    DoubleLinkedList<Song*> searchSongs(const QString &in) const; 
    ~Playlist(); 
};
#endif
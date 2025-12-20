#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include "playlist.h"
#include "doublelinkedlist.h"
#include "song.h" 

class MusicPlayer{ 
private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput; 
    DoubleLinkedList<Playlist*> playlists; 
    Playlist *APlist; 
    int currentIndex; 

public:
    MusicPlayer(); 
    MusicPlayer(Playlist* Plist); 
    ~MusicPlayer(); 
    QMediaPlayer* getMediaPlayer(); 
    QAudioOutput* getAudioOutput() { return audioOutput; }
    void addPlist(Playlist* Plist);
    void removePlist(const QString& name); 
    void setAPlist(const QString& name); 
    void setSource(const QUrl &source);
    Playlist* getAPlist() const; 
    
    void play(int index); 
    void stop(); 
    void next(); 
    void previous(); 
    void pause(); 
};

#endif
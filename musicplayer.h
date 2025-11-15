#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include "playlist.h"
class MusicPlayer {
private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    Playlist *playlist;
    int currentIndex;
public:
    MusicPlayer(Playlist *playlist);
    ~MusicPlayer();
    void play(int index);
    void stop();
    void next();
    void previous();
    void pause();

    void setPlaylist(Playlist* newPlaylist);
};
#endif 

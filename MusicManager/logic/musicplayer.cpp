#include "musicplayer.h"
#include <QUrl>
#include <QDebug>
MusicPlayer::MusicPlayer(Playlist *playlist)
    : playlist(playlist), currentIndex(0)
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    audioOutput->setVolume(1.0); 
    player->setAudioOutput(audioOutput);
}
MusicPlayer::~MusicPlayer(){
    delete player;
    delete audioOutput;
}
void MusicPlayer::play(int index){
    if(!playlist) return;
    const auto& songs = playlist->getSongs();
    if(index < 0 || index >= songs.getSize()) return;
    currentIndex = index;
    player->setSource(QUrl::fromLocalFile(songs.at(index).getFilePath()));
    qDebug() << "Playing:" << songs.at(index).getTitle();
    player->play();
}
void MusicPlayer::stop(){
    player->stop();
}
void MusicPlayer::next(){
    const auto& songs = playlist->getSongs();
    if(songs.getSize() == 0) return;
    currentIndex = (currentIndex + 1) % songs.getSize();
    play(currentIndex);
}
void MusicPlayer::previous(){
    const auto& songs = playlist->getSongs();
    if(songs.getSize() == 0) return;
    currentIndex = (currentIndex - 1 + songs.getSize()) % songs.getSize();
    play(currentIndex);
}

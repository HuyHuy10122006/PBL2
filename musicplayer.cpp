#include "musicplayer.h"
#include <QUrl>
#include <QDebug>
MusicPlayer::MusicPlayer(Playlist *playlist)
    : playlist(playlist), currentIndex(0)
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
}

MusicPlayer::~MusicPlayer() {
    delete player;
    delete audioOutput;
}

void MusicPlayer::play(int index) {
    if (!playlist) return;
    auto songs = playlist->getSongs();
    if (index < 0 || index >= songs.getSize()) return;
    currentIndex = index;
    player->setSource(QUrl::fromLocalFile(songs(index)->getFilePath()));
    qDebug() << "Playing:" << songs(index)->getTitle();
    player->play();
}

void MusicPlayer::stop() {
    player->stop();
}

void MusicPlayer::next() {
    auto songs = playlist->getSongs();
    if (songs.isEmpty()) {
    qDebug() << "Playlist is empty!";
    return;
}
    currentIndex = (currentIndex + 1) % songs.getSize();
    play(currentIndex);
}

void MusicPlayer::previous() {
    auto songs = playlist->getSongs();
    if (songs.isEmpty()) return;
    currentIndex = (currentIndex - 1 + songs.getSize()) % songs.getSize();
    play(currentIndex);
}

void MusicPlayer::pause() {
    player->pause();
}

void MusicPlayer::setPlaylist(Playlist* newPlaylist) {
    if(!newPlaylist) throw std::invalid_argument("Playlist cannot be null");
    playlist = newPlaylist;
    currentIndex = 0;
}

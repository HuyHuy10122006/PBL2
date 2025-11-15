#include "musicManager.h"
#include <stdexcept>
#include <QDebug>

MusicManager::MusicManager() : player(nullptr) {}

MusicManager::~MusicManager() {
    for (int i = 0; i < playlists.getSize(); ++i) {
        delete playlists(i);
    }
    if(player){
        player->stop();
        delete player;}
}

void MusicManager::addPlaylist(Playlist* playlist) {
    if (!playlist)
        throw std::invalid_argument("Cannot add null playlist");
    playlists.append(playlist);
}

void MusicManager::removePlaylist(const QString& name) {
    for (int i = 0; i < playlists.getSize(); ++i) {
        if (playlists(i)->getName().compare(name, Qt::CaseInsensitive) == 0) {
            delete playlists(i);
            playlists.removeAt(i);
            qDebug() << "Removed playlist:" << name;
            return;
        }
    }
    throw std::runtime_error("Playlist not found: " + name.toStdString());
}

Playlist* MusicManager::getPlaylist(const QString& name) const {
    for (int i = 0; i < playlists.getSize(); ++i) {
        if (playlists(i)->getName().compare(name, Qt::CaseInsensitive) == 0)
            return playlists(i);
    }
    qDebug() << "không tìm thấy Playlist!";
    return nullptr;
}

DoubleLinkedList<Playlist*>& MusicManager::getPlaylists() {
    return playlists;
}

void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song) {
    if (!song)
        throw std::invalid_argument("Cannot add null song");
    Playlist* pl = getPlaylist(playlistName);
    pl->addSong(song);
}

void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName){
    Playlist* pl = getPlaylist(playlistName);
    pl->removeSong(songTitle);
}

DoubleLinkedList<Song*> MusicManager::searchHomeSong(const QString& word) const
{
    DoubleLinkedList<Song*> result;
    for (int i = 0; i < songsOnHome.getSize(); ++i) {
        Song* s = songsOnHome(i);
        if (s->getTitle().startsWith(word, Qt::CaseInsensitive))
            result.append(s);
    }
       for (int i = 0; i < playlists.getSize(); ++i) {
        Playlist* pl = playlists(i);
        for (int j = 0; j < pl->getSongs().getSize(); ++j) {
            Song* s = pl->getSongs()(j);
            if (s->getTitle().startsWith(word, Qt::CaseInsensitive))
                result.append(s);
        }
    }
    return result;
}

void MusicManager::playSong(const QString& playlistName, int index) {
    Playlist* pl = nullptr;
    try {
        pl = getPlaylist(playlistName);
    } catch (const std::runtime_error& e) {
        qDebug() << e.what();
        return;
    }

    if (pl->getSongs().getSize() == 0) {
        qDebug() << "Playlist is empty!";
        return;
    }

    if (index < 0 || index >= pl->getSongs().getSize()) {
        qDebug() << "Invalid song index!";
        return;
    }

    if (!player)
        player = new MusicPlayer(pl);
    else
        player->setPlaylist(pl);

    player->play(index);
}

void MusicManager::stop() {
    if (player)
        player->stop();
}

void MusicManager::pause() {
    if (player)
        player->pause();
}

void MusicManager::next() {
    if (player)
        player->next();
}

void MusicManager::previous() {
    if (player)
        player->previous();
}

void MusicManager::listAllPlaylists() const {
    qDebug() << "=== All Playlists ===";
    for (int i = 0; i < playlists.getSize(); ++i) {
        Playlist* pl = playlists(i);
        qDebug() << "Playlist:" << pl->getName()
                 << ", songs:" << pl->getSongs().getSize();
    }
}

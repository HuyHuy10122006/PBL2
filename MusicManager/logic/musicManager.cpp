#include "musicManager.h"
#include <stdexcept>
#include <QDebug>
MusicManager::MusicManager() : player(nullptr){}

MusicManager::~MusicManager(){
    if(player){
        player->stop();
        delete player; 
    }
    for(int i = 0; i < playlists.getSize(); ++i){
        delete playlists(i);
    }
    for(int i = 0; i < allSongs.getSize(); ++i){
        delete allSongs(i);
    }
}

Playlist* MusicManager::getPlaylist(const QString& name) const{
    for(int i = 0; i < playlists.getSize(); ++i){
        Playlist* p = playlists(i);
        if(p->getName().compare(name, Qt::CaseInsensitive) == 0)
            return p;
    }
    return nullptr;
}

void MusicManager::addSongToCatalog(Song* song){
    if(!song) throw std::invalid_argument("Cannot add null song to Catalog.");
    allSongs.append(song); 
    qDebug() << "Added song to Catalog:" << song->getTitle();
}

void MusicManager::addSongToHome(Song* song){
    if(!song) throw std::invalid_argument("Cannot add null song to Home.");
    songsOnHome.append(song); 
    qDebug() << "Added song to Home:" << song->getTitle();
}
MusicPlayer* MusicManager::getPlayer() const{
    return player;
}
void MusicManager::playSong(const QString& playlistName, int index){
    Playlist* pl = getPlaylist(playlistName);
    if(!pl || pl->getSongs().getSize() == 0){
        qDebug() << "Cannot play: Playlist not found or is empty.";
        return;
    }
    if(!player){
        player = new MusicPlayer(pl); 
    } 
    else{
        player->setAPlist(pl->getName());
    }

    player->play(index);
}
void MusicManager::stop(){
    if (player) player->stop();
}

void MusicManager::next(){
    if (player) player->next();
}

void MusicManager::previous(){
    if (player) player->previous();
}

void MusicManager::pause(){
    if (player) player->pause();
}
void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song){
    if(!song)
        throw std::invalid_argument("Cannot add null song");
    Playlist* pl = getPlaylist(playlistName);
    if(pl)
        pl->addSong(song);
}
void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName){
    Playlist* pl = getPlaylist(playlistName);
    if(!pl){
        qDebug() << "Playlist not found:" << playlistName;
        return;
    }
    pl->removeSong(songTitle, artistName);
}
DoubleLinkedList<Song*> MusicManager::searchHomeSong(const QString& word) const {
    DoubleLinkedList<Song*> result;
    for(int i = 0; i < songsOnHome.getSize(); ++i){
        Song* s = songsOnHome(i);
        if (s->getTitle().contains(word, Qt::CaseInsensitive) || 
            s->getArtist().contains(word, Qt::CaseInsensitive))
            result.append(s);
    }
    return result;
}

void MusicManager::playSingleSong(const QString& songTitle, const QString& artistName){
    Song* foundSong = nullptr;
    for(int i = 0; i < allSongs.getSize(); ++i){
        Song* s = allSongs(i);
        if (s->getTitle().compare(songTitle, Qt::CaseInsensitive) == 0 &&
            s->getArtist().compare(artistName, Qt::CaseInsensitive) == 0){
            foundSong = s;
            break;
        }
    }
    if(!foundSong){
        qDebug() << "Song not found in Catalog:" << songTitle << "by" << artistName;
        return;
    }
    Playlist* tempPlaylist = new Playlist("Single Play"); 
    tempPlaylist->addSong(foundSong); 
    if(!player){
        player = new MusicPlayer(tempPlaylist);
        player->addPlist(tempPlaylist); 
        player->setAPlist(tempPlaylist->getName());
    }else{
        player->addPlist(tempPlaylist); 
        player->setAPlist(tempPlaylist->getName());
    }
    player->play(0); 
    qDebug() << "Playing single song:" << foundSong->getTitle();
}
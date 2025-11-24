#include "musicManager.h"
#include <stdexcept>
#include <QDebug>
//Hằng số tên Playlist Tạm thời
const QString SINGLE_PLAY_QUEUE_NAME = "SINGLE_PLAY_QUEUE";
MusicManager::MusicManager() : Aplayer(nullptr){}

MusicManager::~MusicManager(){
    for(int i = 0; i < playlists.getSize(); ++i){
        delete playlists(i);
    }
    for(int i = 0; i < allSongs.getSize(); i++)
    {
        delete allSongs(i);
    }
   for(int i = 0; i < players.getSize(); ++i){
        players(i)->stop(); 
        delete players(i);
    }
}
void MusicManager::addSCatalog(Song* song)
{
    if(!song) throw std::invalid_argument("Khong the them bai hat rong vao catalog!");
    for(int i = 0; i < allSongs.getSize(); i++)
    {
        if(*(allSongs(i)) == *song)
        {
            qDebug() << "Bai hat da ton tai trong catalog!";
            delete song;
            return;
        }
    }
    allSongs.append(song);
    qDebug()<<"bai hat da duoc them vao catalog.";
}
void MusicManager::addSHome(Song* song)
{
    if (!song) return; 

    for (int i = 0; i < songsOnHome.getSize(); ++i) {
        if (songsOnHome(i) == song) {
            qDebug() << song->getTitle() << "Da co trong home.";
            return;
        }
    }

    songsOnHome.append(song);
    qDebug() << "bai hat da duoc them vao home";
}
Song* MusicManager::getSong(const QString& title, const QString& artist) const
{
   for(int i = 0; i < allSongs.getSize(); ++i){
        Song* s = allSongs(i);
        if (s->getTitle().compare(title, Qt::CaseInsensitive) == 0 &&
            s->getArtist().compare(artist, Qt::CaseInsensitive) == 0) {
            return s;
        }
    }
    return nullptr;
}
MusicPlayer* MusicManager::createPlayer()
{
    MusicPlayer* newPlayer = new MusicPlayer();
    players.append(newPlayer);
    if(!Aplayer)
    {
        Aplayer = newPlayer;
    }
    qDebug() <<"Tao va quan ly player moi.";
    return newPlayer;
}
void MusicManager::setAplayer(MusicPlayer* player)
{
    Aplayer = player;
}
MusicPlayer* MusicManager::getAplayer() const {
    return Aplayer;
}
void MusicManager::addPlaylist(Playlist* playlist){
    if(!playlist)
        throw std::invalid_argument("Cannot add null playlist");
        for(int i = 0; i < playlists.getSize(); ++i){
        if(*(playlists(i)) == *playlist){
            qDebug() << "Playlist da ton tai";
            delete playlist; 
            return;
        }
    }
    playlists.append(playlist);
}

void MusicManager::removePlaylist(const QString& name){
    for(int i = 0; i < playlists.getSize(); ++i){
        if(playlists(i)->getName() == name){
            delete playlists(i);
            playlists.removeAt(i);
            qDebug() << "Da xoa Playlist: " << name;
            return;
        }
    }
    throw std::runtime_error("Khong tim thay Playlist: " + name.toStdString());
}

Playlist* MusicManager::getPlaylist(const QString& name) const{
    for(int i = 0; i < playlists.getSize(); ++i){
        if(playlists(i)->getName() == name)
            return playlists(i);
    }
    qDebug() << "khong tim thay Playlist!";
    return nullptr;
}

DoubleLinkedList<Playlist*>& MusicManager::getPlaylists(){
    return playlists;
}

void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song){
    if(!song)
        throw std::invalid_argument("khong the them bai hat rong!");
    Playlist* pl = getPlaylist(playlistName);
    pl->addSong(song);
}

void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName){
    Playlist* pl = getPlaylist(playlistName);
    if(!pl){
        qDebug() << "Khong tim thay Playlist:" << playlistName;
        return;
    }
    pl->removeSong(songTitle, artistName);
}


DoubleLinkedList<Song*> MusicManager::searchHomeSong(const QString& word) const
{
  DoubleLinkedList<Song*> result;
    for(int i = 0; i < allSongs.getSize(); ++i){
        Song* s = allSongs(i);
        if (s->getTitle().contains(word, Qt::CaseInsensitive) || 
            s->getArtist().contains(word, Qt::CaseInsensitive))
            result.append(s);
    }
    return result;
}
DoubleLinkedList<Playlist*> MusicManager::searchPlaylist(const QString& word) const
{
    DoubleLinkedList<Playlist*> results;
    if (word.isEmpty()) return results;
    for (int i = 0; i < playlists.getSize(); ++i) {
        Playlist* pl = playlists(i);
        if (pl->getName().contains(word, Qt::CaseInsensitive)) {
            results.append(pl);
        }
    }
    return results;
}
void MusicManager::playSong(const QString& playlistName, int index){
    Playlist* pl = nullptr;
    try{
        pl = getPlaylist(playlistName);
    }catch (const std::runtime_error& e){
        qDebug() << e.what();
        return;
    }

   if (!pl) return;
    if (!Aplayer) { 
        Aplayer = createPlayer(); 
    }
    
    if(pl->getSongs().getSize() == 0){
        qDebug() << "Playlist rong khong the phat!";
        return;
    }
    Aplayer->addPlist(pl); 
    Aplayer->setAPlist(pl->getName());
    Aplayer->play(index);
}

void MusicManager::playSingleSong(const QString& songTitle, const QString& artistName){

    Song* targetSong = getSong(songTitle, artistName);
    if (!targetSong) {
        qDebug() << "Song not found in Catalog:" << songTitle << "by" << artistName;
        return;
    }
    Playlist* tempQueue = getPlaylist(SINGLE_PLAY_QUEUE_NAME);

    if (!tempQueue) {
        tempQueue = new Playlist(SINGLE_PLAY_QUEUE_NAME);
        tempQueue->setTemporary(true); 
        
        playlists.append(tempQueue); 
    }
    tempQueue->getSongs().clear(); 
    tempQueue->addSong(targetSong); 
    if (!Aplayer) { 
        Aplayer = createPlayer(); 
    }
    Aplayer->addPlist(tempQueue); 
    Aplayer->setAPlist(tempQueue->getName());
    Aplayer->play(0); 
    
    qDebug() << "Playing single song:" << targetSong->getTitle() << "via temporary queue.";
}

void MusicManager::stop(){
    if(Aplayer)
        Aplayer->stop();
}

void MusicManager::pause(){
    if(Aplayer)
        Aplayer->pause();
}

void MusicManager::next(){
    if(Aplayer)
        Aplayer->next();
}

void MusicManager::previous(){
    if(Aplayer)
        Aplayer->previous();
}
void MusicManager::listAllPlaylists() const{
  qDebug() << "All Playlists";
    for(int i = 0; i < playlists.getSize(); ++i){
        Playlist* pl = playlists(i);
        qDebug() << "Playlist:" << pl->getName()
                 << ", songs:" << pl->getSongs().getSize();
    }
}

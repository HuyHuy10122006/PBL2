#include "musicplayer.h"
#include <QUrl>
#include <QDebug>

MusicPlayer::MusicPlayer()
    :APlist(nullptr), currentIndex(0)
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);  
}

MusicPlayer::MusicPlayer(Playlist* Plist)
    :APlist(Plist), currentIndex(0)
{
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
}
MusicPlayer::~MusicPlayer(){
// Xóa các Playlist tạm thời do MusicPlayer tạo ra
    for(int i = 0; i < playlists.getSize(); ++i){
        if(playlists(i)->getIsTemporary()){
            delete playlists(i);
        }
    }
    delete player;
    delete audioOutput;
}

void MusicPlayer::addPlist(Playlist* Plist)
{
    if(!Plist) throw std::invalid_argument("Playlist khong the rong!");
    for(int i = 0; i < playlists.getSize(); i++)
    {
        if(*(playlists(i)) == *Plist){
            qDebug() << "Playlist da ton tai!";
            return;
        }
    }
    playlists.append(Plist);
    if(!APlist)
    {
        APlist = Plist;
        currentIndex = 0;
    }
    qDebug() << "Them Playlist " << Plist->getName() <<" vao trinh phat nhac.";
}
void MusicPlayer::removePlist(const QString& name)
{
    for(int i = 0; i < playlists.getSize(); i++)
    {
        if(playlists(i)->getName() == name)
        {
            if(playlists(i) == APlist)
            {
                APlist = nullptr;
                player->stop();
                currentIndex = 0;
            }
            playlists.removeAt(i); 
            qDebug() << "Xoa Playlist " << name << " khoi danh sach";
            return;
        }
    }
    qDebug() <<"Khong tim thay Playlist can xoa!"; 
}
void MusicPlayer::setAPlist(const QString& name)
{
    for(int i = 0; i < playlists.getSize(); i++)
    {
        if(playlists(i)->getName() == name)
        {
            APlist = playlists(i);
            currentIndex = 0;
            qDebug()<<"Dat Playlist hoat dong la:" << name;
            return;
        }
    }
    qDebug()<<"Khong tim thay Playlist co ten " << name;
}

Playlist* MusicPlayer::getAPlist() const{
    return this->APlist;
}
void MusicPlayer::play(int index){
    if(!APlist) return;
    auto songs = APlist->getSongs();
    if(index < 0 || index >= songs.getSize()) return;

    currentIndex = index;
    Song* currentSong = songs(index);
    int newCount = currentSong->getPlayCount() + 1; 
    currentSong->setPlayCount(newCount);           
    
    qDebug() << "PlayCount updated for:" << currentSong->getTitle() << "| New Count:" << newCount; 
    player->setSource(QUrl::fromLocalFile(currentSong->getFilePath()));
    qDebug() << "Playing:" << currentSong->getTitle();
    player->play();
}

void MusicPlayer::stop(){
    player->stop();
}

void MusicPlayer::next(){
    if(!APlist) return;
    auto songs = APlist->getSongs();
    if(songs.isEmpty()){
    qDebug() << "Playlist is empty!";
    return;
}
    currentIndex = (currentIndex + 1) % songs.getSize();
    play(currentIndex);
}

void MusicPlayer::previous(){
    if(!APlist) return;
    auto songs = APlist->getSongs();
    if(songs.isEmpty()) return;
    currentIndex = (currentIndex - 1 + songs.getSize()) % songs.getSize();
    play(currentIndex);
}

void MusicPlayer::pause(){
    player->pause();
}

#include "playlist.h"
Playlist::Playlist(const QString &name) : name(name){}
void Playlist::addSong(const Song &song){
    songs.append(song);
}
void Playlist::removeSong(const QString &title){
    for(int i = 0; i < songs.getSize(); ++i){
        if(songs.at(i).getTitle().compare(title, Qt::CaseInsensitive) == 0){
            songs.removeAt(i);
            qDebug() << "Removed:" << title;
            return;
        }
    }
    qDebug() << "Song not found:" << title;
}

void Playlist::listSongs() const{
    qDebug() << "\n=== Playlist:" << name << "===";
    for(int i = 0; i < songs.getSize(); ++i){
        qDebug().noquote() << songs.at(i).toString();
    }
    qDebug() << "Total duration:" << totalDuration() << "seconds\n";
}

int Playlist::totalDuration() const{
    int sum = 0;
    for(int i = 0; i < songs.getSize(); ++i)
        sum += songs.at(i).getDuration();
    return sum;
}
const DoubleLinkedList<Song>& Playlist::getSongs() const{
    return songs;
}

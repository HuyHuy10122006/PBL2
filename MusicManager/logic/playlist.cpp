#include "playlist.h"
using namespace std;
Playlist::Playlist(const QString &name) : name(name){}

bool Playlist::operator ==(const Playlist &p) const
{
    return name = p.getName();
}

QString Playlist::getName() const
{
    return this->name;
}


bool Playlist::isTemporaryPlaylist() const{
    return isTemporary;
}
void Playlist::addSong(Song* song){
    if(!song)
        throw invalid_argument("Cannot add a null song!");
    for(int i = 0 ; i < songs.getSize(); i++)
    {
        if(*(songs(i)) == *song) {
            qDebug() << "Bai hat " << song->getTitle() << "da co trong playlist";
            return;
        }
    }
    songs.append(song);
    qDebug() << "them bai hat" << song->getTitle() << "thanh cong";
}

void Playlist::removeSong(const QString &title, const QString &artist){
    if(songs.getSize() == 0) {
        qDebug() << "Playlist is empty!";
        return;
    }

    for(int i = 0; i < songs.getSize(); ++i){
        Song* s = songs(i);
        if (s->getTitle().compare(title, Qt::CaseInsensitive) == 0 &&
            s->getArtist().compare(artist, Qt::CaseInsensitive) == 0) {
            delete s;              
            songs.removeAt(i); 
            qDebug() << "Removed song:" << title << "by" << artist;
            return;
        }
    }
    qDebug() << "Song not found:" << title << "by" << artist;
}



void Playlist::listSongs() const{
    qDebug() << "\n=== Playlist:" << name << "===";

    if(songs.getSize() == 0){
        qDebug() << "Playlist is empty.\n";
        return;
    }

    for (int i = 0; i < songs.getSize(); ++i)
        qDebug().noquote() << songs(i)->toString();

    qDebug() << "Total duration:" << totalDuration() << "seconds\n";
}

int Playlist::totalDuration() const{
    int sum = 0;
    for(int i = 0; i < songs.getSize(); ++i)
        sum += songs(i)->getDuration();
    return sum;
}

bool Playlist::isEmpty() const{
    return songs.getSize() == 0;
}
const DoubleLinkedList<Song*>&Playlist::getSongs() const{
    return songs;
}
DoubleLinkedList<Song*>Playlist::searchSongs(const QString &in) const
{
    DoubleLinkedList<Song*> results;

    if(in.isEmpty()) return results;
    for(int i = 0 ; i < songs.getSize(); i++)
    {
        Song* s = songs(i);
        if(s->getTitle().compare(in, Qt::CaseInsensitive) == 0)
        {
            results.clear();
            results.append(s);
            break;
        }
        if(s->getTitle().startsWith(in, Qt::CaseInsensitive))
        {
            results.append(s);
        }
    }
    return results;
}

Playlist::~Playlist(){
    for(int i = 0; i < songs.getSize(); ++i)
        delete songs(i);
}

#include "playlist.h"
using namespace std;

Playlist::Playlist(const QString &name) : name(name) {}


void Playlist::addSong(Song* song) {
    if (!song)
        throw invalid_argument("Cannot add a null song!");
    for (int i = 0 ; i < songs.getSize(); i++)
    {
        if(*(songs(i)) == *song) {
            qDebug() << "Bai hat " << song->getTitle() << "da co trong playlist";
            return;
        }
    }
    songs.append(song);
    qDebug() << "them bai hat" << song->getTitle() << "thanh cong";
}
QString Playlist::getName() const
{
    return this->name;
}

void Playlist::removeSong(const QString &title, const QString &artist) {
    if (songs.getSize() == 0)
        throw runtime_error("Playlist is empty!");

    for (int i = 0; i < songs.getSize(); ++i) {
        Song* rms = songs(i);
        if (songs(i)->getTitle().compare(title, Qt::CaseInsensitive) == 0 &&
            songs(i)->getArtist().compare(artist, Qt::CaseInsensitive) ==0) {
            songs.removeAt(i);      // giải phóng bộ nhớ
            delete rms;
            qDebug() << "Removed:" << title;
            return;
        }
    }
    qDebug() << "Khong co bai hat tuong ung:" << title;
}


void Playlist::listSongs() const {
    qDebug() << "\n=== Playlist:" << name << "===";

    if (songs.getSize() == 0) {
        qDebug() << "Playlist is empty.\n";
        return;
    }

    for (int i = 0; i < songs.getSize(); ++i)
        qDebug().noquote() << songs(i)->toString();

    qDebug() << "Total duration:" << totalDuration() << "seconds\n";
}

int Playlist::totalDuration() const {
    int sum = 0;
    for (int i = 0; i < songs.getSize(); ++i)
        sum += songs(i)->getDuration();
    return sum;
}

bool Playlist::isEmpty() const {
    return songs.getSize() == 0;
}

const DoubleLinkedList<Song*>&Playlist::getSongs() const {
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

Playlist::~Playlist() {
    for (int i = 0; i < songs.getSize(); ++i)
        delete songs(i);
}

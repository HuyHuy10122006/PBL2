#include "song.h"
Song::Song() : duration(0){}
Song::Song(const QString &title,const QString &artist,int duration,const QString &filePath)
    : title(title),artist(artist),duration(duration),filePath(filePath){}
QString Song::getTitle() const { return title; }
QString Song::getArtist() const { return artist; }
int Song::getDuration() const { return duration; }
QString Song::getFilePath() const { return filePath; }
void Song::setTitle(const QString &t) { title = t; }
void Song::setArtist(const QString &a) { artist = a; }
void Song::setDuration(int d) { duration = d; }
void Song::setFilePath(const QString &path) { filePath = path; }
QString Song::toString() const{
    return QString("%1 - %2 (%3s)\nFile: %4")
        .arg(artist)
        .arg(title)
        .arg(duration)
        .arg(filePath);
}

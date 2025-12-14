#include "song.h"
#include <QDate> 
using namespace std;
Song::Song() : title(""), artist(""), duration(0), filePath(""), playCount(0), dateAdded(QDate::currentDate()){}
Song::Song(const QString &title, const QString &artist, int duration, const QString &filePath, QDate dateAdded)
{
    if(title.isEmpty() || artist.isEmpty()) throw invalid_argument("Title and artist cannot be empty!");
    if(duration < 0) throw invalid_argument("Duration cannot be negative!");
    
    this->title = title;
    this->artist = artist;
    this->duration = duration;
    this->filePath = filePath;
    this->playCount = 0; 
    this->dateAdded = dateAdded;
}

QString Song::printTime(int s) const{
    int m = s / 60;
    int sec = s % 60;
    return QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(sec, 2, 10, QChar('0'));
}
bool Song::operator ==(const Song &s) const
{
    return title == s.title &&
           artist == s.artist &&
           filePath == s.filePath;
}

// GETTERS 
QString Song::getTitle() const { return title; }
QString Song::getArtist() const { return artist; }
int Song::getDuration() const { return duration; }
QString Song::getFilePath() const { return filePath; }
int Song::getPlayCount() const { return playCount; }
QDate Song::getDateAdded() const { return dateAdded; }

// SETTERS
void Song::setTitle(const QString& t){
    if(t.isEmpty()) throw std::invalid_argument("Title cannot be empty");
    title = t;
}
void Song::setArtist(const QString& a){
    if(a.isEmpty()) throw std::invalid_argument("Artist cannot be empty");
    artist = a;
}
void Song::setDuration(int d) {
    if(d < 0) throw std::invalid_argument("Duration cannot be negative");
    duration = d;
}
void Song::setFilePath(const QString &path) { filePath = path; }

void Song::setPlayCount(int count) {
    if(count < 0) throw std::invalid_argument("Play count cannot be negative");
    playCount = count;
}

QString Song::toString() const{
    return QString("%1 - %2 (%3)\nFile: %4\nPlays: %5 | Added: %6")
        .arg(artist)
        .arg(title)
        .arg(printTime(duration))
        .arg(filePath)
        .arg(playCount)
        .arg(dateAdded.toString("yyyy-MM-dd"));
}
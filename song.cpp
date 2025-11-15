#include "song.h"
using namespace std;
Song::Song() : title(""), artist(""), duration(0), filePath(""){} 

Song::Song(const QString &title, const QString &artist, int duration, const QString &filePath)
{
    if(title.isEmpty() || artist.isEmpty()) throw invalid_argument("Title and artist cannot be empty!");
    if(duration < 0) throw invalid_argument("Duration cannot be negative!");
    this->title = title;
    this->artist = artist;
    this->duration = duration;
    this->filePath = filePath;
}

QString Song::printTime(int s) const{
    int m = s / 60;
    int sec = s % 60;
    return QString("%1:%2")
        .arg(m, 2, 10, QChar('0'))
        .arg(sec, 2, 10, QChar('0'));
}
bool Song::operator ==(const Song &other) const
{
    return title == other.title &&
           artist == other.artist &&
           filePath == other.filePath;
}

QString Song::getTitle() const { return title; }
QString Song::getArtist() const { return artist; }
int Song::getDuration() const { return duration; }
QString Song::getFilePath() const { return filePath; }


void Song::setTitle(const QString& t) {
    if (t.isEmpty()) throw std::invalid_argument("Title cannot be empty");
    title = t;
}

void Song::setArtist(const QString& a) {
    if (a.isEmpty()) throw std::invalid_argument("Artist cannot be empty");
    artist = a;
}

void Song::setDuration(int d) {
    if (d < 0) throw std::invalid_argument("Duration cannot be negative");
    duration = d;
}

void Song::setFilePath(const QString &path) { filePath = path; }


QString Song::toString() const {
    return QString("%1 - %2 (%3)\nFile: %4")
        .arg(artist)
        .arg(title)
        .arg(printTime(duration))
        .arg(filePath);
}

#ifndef SONG_H
#define SONG_H
#include <QString>
#include <stdexcept>
class Song {
private:
    QString title;
    QString artist;
    int duration;   //giây
    QString filePath;
public:
    Song();
    Song(const QString &title, const QString &artist, int duration, const QString &filePath);

    QString printTime(int s) const;
    bool operator ==(const Song &other) const;

    QString getTitle() const;
    QString getArtist() const;
    int getDuration() const;
    QString getFilePath() const;


    void setTitle(const QString &t);
    void setArtist(const QString &a);
    void setDuration(int d);
    void setFilePath(const QString &path);


    QString toString() const;
};
#endif

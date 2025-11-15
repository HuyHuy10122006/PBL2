#ifndef SONG_H
#define SONG_H
#include <QString>
#include <QDebug>
class Song{
private:
    QString title;
    QString artist;
    int duration;   
    QString filePath;
public:
    Song();
    Song(const QString &title,const QString &artist,int duration,const QString &filePath);

    QString getTitle() const;
    QString getArtist() const;
    int getDuration() const;
    QString getFilePath() const;

    void setTitle(const QString &t);
    void setArtist(const QString &a);
    void setDuration(int d);
    void setFilePath(const QString &path);

    QString toString() const;
    friend QDebug operator<<(QDebug dbg, const Song &song){
        dbg.noquote() << song.toString();
        return dbg;
    }
};

#endif

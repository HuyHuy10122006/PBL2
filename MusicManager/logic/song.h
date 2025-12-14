#ifndef SONG_H
#define SONG_H
#include <QString>
#include <QDate> 
#include <stdexcept>

class Song{
private:
    QString title; 
    QString artist; 
    int duration; 
    QString filePath;
    int playCount;   // Số lần phát
    QDate dateAdded; // Ngày thêm vào Catalog
public:
    Song();
    Song(const QString &title, const QString &artist, int duration, const QString &filePath, QDate dateAdded = QDate::currentDate()); 
    
    QString printTime(int s) const; //phút:giây
    bool operator ==(const Song &other) const; // so sánh 2 song

    QString getTitle() const; 
    QString getArtist() const; 
    int getDuration() const; 
    QString getFilePath() const;
    int getPlayCount() const;
    QDate getDateAdded() const;

    void setPlayCount(int count);
    void setTitle(const QString &t);
    void setArtist(const QString &a);
    void setDuration(int d);
    void setFilePath(const QString &path);
    QString toString() const; 
};
#endif
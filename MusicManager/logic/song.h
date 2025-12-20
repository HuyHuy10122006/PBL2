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
    QString coverPath; // [MỚI] Đường dẫn ảnh bìa
    int playCount;   
    QDate dateAdded; 
public:
    Song();
    Song(const QString &title, const QString &artist, int duration, const QString &filePath, const QString &coverPath = "", QDate dateAdded = QDate::currentDate()); 
    
    QString printTime(int s) const; 
    bool operator ==(const Song &other) const; 

    QString getTitle() const; 
    QString getArtist() const; 
    int getDuration() const; 
    QString getFilePath() const;
    QString getCoverPath() const; // [MỚI]
    int getPlayCount() const;
    QDate getDateAdded() const;

    void setPlayCount(int count);
    void setTitle(const QString &t);
    void setArtist(const QString &a);
    void setDuration(int d);
    void setFilePath(const QString &path);
    void setCoverPath(const QString &path); // [MỚI]
    QString toString() const; 
};
#endif
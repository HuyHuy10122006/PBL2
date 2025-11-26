#ifndef SONG_H
#define SONG_H
#include <QString>
#include <stdexcept>
class Song{
private:
    QString title;   
    QString artist; 
    int duration;     
    QString filePath; 
public:
    Song(); 
    Song(const QString &title,const QString &artist,int duration,const QString &filePath); 
    
    QString printTime(int s) const; //phút:giây
    bool operator ==(const Song &other) const; // so sánh 2 song

    QString getTitle() const; 
    QString getArtist() const;   
    int getDuration() const;     
    QString getFilePath() const;  

    void setTitle(const QString &t);
    void setArtist(const QString &a);
    void setDuration(int d);
    void setFilePath(const QString &path);
    QString toString() const; // Trả về chuỗi mô tả đầy đủ thông tin bài hát
};
#endif
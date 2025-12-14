#include "musicManager.h"
#include <stdexcept>
#include <QDebug>
#include <QRandomGenerator> 
#include <algorithm> // Giữ lại cho std::min và std::function
#include <QFile> 
#include <QTextStream> 
#include <QDate> 
#include <functional> // Cần cho std::function
DoubleLinkedList<Song*> sortDLL(const DoubleLinkedList<Song*>& original, 
                                 std::function<bool(const Song*, const Song*)> comparator) {
    
    // TẠO BẢN SAO (quan trọng để không sửa đổi allSongs gốc)
    DoubleLinkedList<Song*> sorted = original; 

    int n = sorted.getSize();

    // Sắp xếp bằng Bubble Sort
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // Nếu comparator trả về true (cần hoán đổi vị trí)
            if (comparator(sorted(j), sorted(j + 1))) {
                // Hoán đổi (swap) nội dung của hai Song*
                Song* temp = sorted(j);
                sorted(j) = sorted(j + 1);
                sorted(j + 1) = temp;
            }
        }
    }
    return sorted;
}

void MusicManager::saveData() const {
    QFile file("catalog.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file for writing:" << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << "Title,Artist,FilePath,PlayCount,DateAdded\n";

    for (int i = 0; i < allSongs.getSize(); ++i) {
        Song* song = allSongs(i);
        out << "\"" << song->getTitle().replace('"', "\"\"") << "\"," 
            << "\"" << song->getArtist().replace('"', "\"\"") << "\","
            << "\"" << song->getFilePath().replace('"', "\"\"") << "\","
            << song->getPlayCount() << ","
            << song->getDateAdded().toString("yyyy-MM-dd") << "\n";
    }

    qDebug() << "Data saved successfully to catalog.csv." << allSongs.getSize() << "songs saved.";
    file.close();
}

void MusicManager::loadData() {
    QFile file("catalog.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Catalog file not found or could not be opened. Starting with empty catalog.";
        return;
    }
    
    QTextStream in(&file);
    QString line = in.readLine(); 
    
    while (!in.atEnd()) {
        line = in.readLine();
        QStringList fields = line.split(',');
        
        if (fields.size() < 5) continue;

        QString title = fields[0].mid(1, fields[0].length() - 2).replace("\"\"", "\""); 
        QString artist = fields[1].mid(1, fields[1].length() - 2).replace("\"\"", "\"");
        QString filePath = fields[2].mid(1, fields[2].length() - 2).replace("\"\"", "\"");
        
        int playCount = fields[3].toInt();
        QDate dateAdded = QDate::fromString(fields[4].trimmed(), "yyyy-MM-dd");

        for (int i = 0; i < allSongs.getSize(); ++i) {
            Song* existingSong = allSongs(i);
            
            if (existingSong->getTitle() == title && 
                existingSong->getArtist() == artist &&
                existingSong->getFilePath() == filePath) 
            {
                existingSong->setPlayCount(playCount);
                break; 
            }
        }
    }
    
    qDebug() << "Metadata loaded successfully from catalog.csv. Play counts updated.";
    file.close();
} 

MusicManager::MusicManager() : player(nullptr){
    loadData();
}

MusicManager::~MusicManager(){
    saveData();
    
    if(player){
        player->stop();
        delete player; 
    }
    for(int i = 0; i < playlists.getSize(); ++i){
        delete playlists(i);
    }
    for(int i = 0; i < allSongs.getSize(); ++i){
        delete allSongs(i);
    }
}

Playlist* MusicManager::getPlaylist(const QString& name) const{
    for(int i = 0; i < playlists.getSize(); ++i){
        Playlist* p = playlists(i);
        if(p->getName().compare(name, Qt::CaseInsensitive) == 0)
            return p;
    }
    return nullptr;
}

void MusicManager::addSongToCatalog(Song* song){
    if(!song) throw std::invalid_argument("Cannot add null song to Catalog.");
    allSongs.append(song); 
    qDebug() << "Added song to Catalog:" << song->getTitle();
}

void MusicManager::addSongToHome(Song* song){
    if(!song) throw std::invalid_argument("Cannot add null song to Home.");
    songsOnHome.append(song); 
    qDebug() << "Added song to Home:" << song->getTitle();
}
MusicPlayer* MusicManager::getPlayer() const{
    return player;
}

void MusicManager::playSong(const QString& playlistName, int index){
    Playlist* pl = getPlaylist(playlistName);
    if(!pl || pl->getSongs().getSize() == 0){
        qDebug() << "Cannot play: Playlist not found or is empty.";
        return;
    }
    if(!player){
        player = new MusicPlayer();
    } 
    
    if (player->getAPlist() != pl) {
        player->addPlist(pl);
        player->setAPlist(pl->getName());
    }

    player->play(index);
}

void MusicManager::stop(){
    if (player) player->stop();
}

void MusicManager::next(){
    if (player) player->next();
}

void MusicManager::previous(){
    if (player) player->previous();
}

void MusicManager::pause(){
    if (player) player->pause();
}
void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song){
    if(!song)
        throw std::invalid_argument("Cannot add null song");
    Playlist* pl = getPlaylist(playlistName);
    if(pl)
        pl->addSong(song);
}
void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName){
    Playlist* pl = getPlaylist(playlistName);
    if(!pl){
        qDebug() << "Playlist not found:" << playlistName;
        return;
    }
    pl->removeSong(songTitle, artistName);
}
DoubleLinkedList<Song*> MusicManager::searchHomeSong(const QString& word) const {
    DoubleLinkedList<Song*> result;
    for(int i = 0; i < songsOnHome.getSize(); ++i){
        Song* s = songsOnHome(i);
        if (s->getTitle().contains(word, Qt::CaseInsensitive) || 
            s->getArtist().contains(word, Qt::CaseInsensitive))
            result.append(s);
    }
    return result;
}
DoubleLinkedList<Playlist*> MusicManager::searchPlaylist(const QString& word) const{
    DoubleLinkedList<Playlist*> result;
    for(int i = 0; i < playlists.getSize(); ++i){
        Playlist* p = playlists(i);
        if (p->getName().contains(word, Qt::CaseInsensitive))
            result.append(p);
    }
    return result;
}
void MusicManager::playSingleSong(const QString& songTitle, const QString& artistName){
    Song* foundSong = nullptr;
    for(int i = 0; i < allSongs.getSize(); ++i){
        Song* s = allSongs(i);
        if (s->getTitle().compare(songTitle, Qt::CaseInsensitive) == 0 &&
            s->getArtist().compare(artistName, Qt::CaseInsensitive) == 0){
            foundSong = s;
            break;
        }
    }
    if(!foundSong){
        qDebug() << "Song not found in Catalog:" << songTitle << "by" << artistName;
        return;
    }
    
    Playlist* tempPlaylist = new Playlist("Single Play", true);
    tempPlaylist->addSong(foundSong); 
    
    if(!player){
        player = new MusicPlayer();
    }
    
    player->addPlist(tempPlaylist); 
    player->setAPlist(tempPlaylist->getName());
    
    player->play(0); 
    qDebug() << "Playing single song:" << foundSong->getTitle();
}
void MusicManager::addPlaylist(Playlist* playlist){
    if(!playlist) throw std::invalid_argument("Playlist cannot be null");
    playlists.append(playlist);
}

void MusicManager::removePlaylist(const QString& name){
    for(int i = 0; i < playlists.getSize(); ++i){
        if(playlists(i)->getName() == name){
            Playlist* p = playlists(i); 
            if(player && player->getAPlist() == p) {
                player->setAPlist("Default");
            }

            playlists.removeAt(i);
            delete p; 
            qDebug() << "Removed playlist and deleted object:" << name;
            return;
        }
    }
}
DoubleLinkedList<Playlist*>& MusicManager::getPlaylists(){
    return playlists;
}
void MusicManager::listAllPlaylists() const{
    qDebug() << "--- All Playlists ---";
    for(int i = 0; i < playlists.getSize(); ++i){
        qDebug() << i + 1 << ":" << playlists(i)->getName();
    }
}

// Bảng Xếp Hạng (Sắp xếp theo playCount)
DoubleLinkedList<Song*> MusicManager::getTopSongs(int count) const {
    if (allSongs.getSize() == 0) return DoubleLinkedList<Song*>();
    
    // Sắp xếp GIẢM DẦN theo PlayCount (sử dụng Bubble Sort)
    DoubleLinkedList<Song*> sortedList = sortDLL(allSongs, 
        [](const Song* a, const Song* b) {
            return a->getPlayCount() < b->getPlayCount(); 
        });
        
    DoubleLinkedList<Song*> result;
    int limit = std::min(count, sortedList.getSize());
    for (int i = 0; i < limit; ++i) {
        result.append(sortedList(i));
    }
    
    qDebug() << "Generated Top" << limit << "Songs based on play count.";
    return result;
}

//  Mới Phát Hành (Sắp xếp theo dateAdded)
DoubleLinkedList<Song*> MusicManager::getNewReleases(int count) const {
    if (allSongs.getSize() == 0) return DoubleLinkedList<Song*>();
    
    // Sắp xếp GIẢM DẦN theo dateAdded (sử dụng Bubble Sort)
    DoubleLinkedList<Song*> sortedList = sortDLL(allSongs, 
        [](const Song* a, const Song* b) {
            return a->getDateAdded() < b->getDateAdded(); 
        });
        
    DoubleLinkedList<Song*> result;
    int limit = std::min(count, sortedList.getSize());
    for (int i = 0; i < limit; ++i) {
        result.append(sortedList(i));
    }
    
    qDebug() << "Generated Top" << limit << "New Releases based on date added.";
    return result;
}

//  Danh sách theo Ca sĩ
DoubleLinkedList<Song*> MusicManager::getSongsByArtist(const QString& artistName) const {
    DoubleLinkedList<Song*> result;
    
    for (int i = 0; i < allSongs.getSize(); ++i) {
        Song* s = allSongs(i);
        if (s->getArtist().compare(artistName, Qt::CaseInsensitive) == 0) {
            result.append(s);
        }
    }
    
    qDebug() << "Found" << result.getSize() << "songs by artist:" << artistName;
    return result;
}

//  Gợi ý Bài hát (Lấy ngẫu nhiên) - THUẦN DLL
DoubleLinkedList<Song*> MusicManager::getRecommendedSongs(int count) const {
    if (allSongs.getSize() == 0 || count <= 0) return DoubleLinkedList<Song*>();
    
    DoubleLinkedList<Song*> result;
    int n = allSongs.getSize();
    int limit = std::min(count, n);

    // TẠO BẢN SAO CỦA allSongs để chọn ngẫu nhiên và xóa (tránh trùng lặp)
    DoubleLinkedList<Song*> tempSongs = allSongs; 

    for (int i = 0; i < limit; ++i) {
        // Lấy chỉ mục ngẫu nhiên
        int randomIndex = QRandomGenerator::global()->bounded(tempSongs.getSize());
        
        result.append(tempSongs(randomIndex));
        
        // Xóa bài hát khỏi danh sách tạm thời 
        tempSongs.removeAt(randomIndex); 
    }
    
    qDebug() << "Generated" << result.getSize() << "random song recommendations.";
    return result;
}
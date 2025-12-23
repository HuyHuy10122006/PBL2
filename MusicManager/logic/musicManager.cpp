#include "musicManager.h"
#include <QRandomGenerator> 
#include <QFile> 
#include <QTextStream> 
#include <QDate>
#include <functional>
#include <QFileInfo>
#include <QDateTime>
DoubleLinkedList<Song*> sortDLL(const DoubleLinkedList<Song*>& original, std::function<bool(const Song*, const Song*)> comparator) {
    int n = original.getSize();
    DoubleLinkedList<Song*> sorted; 
    if (n == 0) return sorted;

    // 1. Chép con trỏ sang danh sách mới để biệt lập (Yêu cầu DoubleLinkedList có Copy Constructor hoặc dùng vòng lặp này)
    for(int i = 0; i < n; ++i) {
        sorted.append(original(i));
    }

    // 2. Thuật toán sắp xếp (Bubble Sort)
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // So sánh tiêu chí dựa trên comparator truyền vào
            if (comparator(sorted(j), sorted(j + 1))){
                // Hoán đổi con trỏ bài hát giữa các Node
                Song* temp = sorted(j);
                sorted(j) = sorted(j + 1);
                sorted(j + 1) = temp;
            }
        }
    }
    return sorted; // Trả về bản sao đã sắp xếp an toàn
}
 
void MusicManager::saveData() const{
    QFile file("catalog.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "Title,Artist,FilePath,CoverPath,PlayCount,DateAdded\n"; 

    for (int i = 0; i < allSongs.getSize(); ++i) {
        Song* song = allSongs(i);
        out << "\"" << song->getTitle().replace('"', "\"\"") << "\"," 
            << "\"" << song->getArtist().replace('"', "\"\"") << "\","
            << "\"" << song->getFilePath().replace('"', "\"\"") << "\","
            << "\"" << song->getCoverPath().replace('"', "\"\"") << "\","
            << song->getPlayCount() << ","
            << song->getDateAdded().toString("yyyy-MM-dd") << "\n";
    }
    file.close();
}

void MusicManager::loadData(){
    QFile file("catalog.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    in.readLine(); 
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.size() < 6) continue;

        auto clean = [](QString s) { return s.mid(1, s.length() - 2).replace("\"\"", "\""); };
        QString title = clean(fields[0]);
        QString artist = clean(fields[1]);
        QString filePath = clean(fields[2]);
        QString coverPath = clean(fields[3]);
        int playCount = fields[4].toInt();
        QDate dateAdded = QDate::fromString(fields[5].trimmed(), "yyyy-MM-dd");

        Song* s = new Song(title, artist, 0, filePath, coverPath, dateAdded);
        s->setPlayCount(playCount);
        allSongs.append(s);
    }
    file.close();
} 

MusicManager::MusicManager(){ 
    player = new MusicPlayer(); 
    loadData();
    loadPlaylists(); 
}

MusicManager::~MusicManager(){
    saveData();       
    savePlaylists();  
    if(player) {
        player->stop();
        delete player; 
        player = nullptr;
    }
    for(int i = 0; i < playlists.getSize(); ++i){
        delete playlists(i); 
    }
    for(int i = 0; i < allSongs.getSize(); ++i){
        delete allSongs(i);
    }
}

void MusicManager::playSongByObject(Song* s, const DoubleLinkedList<Song*>& currentContext) {
    if (!s || !player || currentContext.isEmpty()) return;

    player->stop();
    
    // 1. Tìm vị trí (index) của bài hát trong danh sách đang hiển thị
    int foundIndex = -1;
    for (int i = 0; i < currentContext.getSize(); ++i) {
        if (currentContext(i) == s) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex != -1) {
        // 2. Tạo hoặc cập nhật Playlist tạm thời "Current_Context"
        QString contextName = "Current_Context";
        Playlist* contextPl = getPlaylist(contextName);
        
        if (contextPl) {
            // Xóa danh sách cũ để nạp danh sách mới của Playlist/Mood vừa nhấn
            const_cast<DoubleLinkedList<Song*>&>(contextPl->getSongs()).clear();
        } else {
            contextPl = new Playlist(contextName, true); // true là playlist tạm
            playlists.append(contextPl);
        }

        // 3. Sao chép các bài hát từ danh sách đang xem vào Playlist của Player
        for(int i = 0; i < currentContext.getSize(); ++i) {
            contextPl->addSong(currentContext(i));
        }

        // 4. Thiết lập cho Player
        player->addPlist(contextPl);
        player->setAPlist(contextName);
        
        // 5. Phát tại đúng vị trí index đã tìm thấy
        player->play(foundIndex);
        
        s->setPlayCount(s->getPlayCount() + 1);
        saveData();
    }
}

void MusicManager::addSongToCatalog(Song* song) { if(song) allSongs.append(song); }
void MusicManager::addSongToHome(Song* song) { if(song) songsOnHome.append(song); }
MusicPlayer* MusicManager::getPlayer() const { return player; }
void MusicManager::stop() { if (player) player->stop(); }
void MusicManager::next() { if (player) player->next(); }
void MusicManager::previous() { if (player) player->previous(); }
void MusicManager::pause() { if (player) player->pause(); }

Playlist* MusicManager::getPlaylist(const QString& name) const {
    for(int i = 0; i < playlists.getSize(); ++i) {
        if(playlists(i)->getName().compare(name, Qt::CaseInsensitive) == 0) return playlists(i);
    }
    return nullptr;
}
DoubleLinkedList<Playlist*>& MusicManager::getPlaylists() {
    return playlists;
}

void MusicManager::addPlaylist(Playlist* p) { if(p) playlists.append(p); }
void MusicManager::removePlaylist(const QString& name) {
    for (int i = 0; i < playlists.getSize(); ++i) {
        if (playlists(i)->getName().compare(name, Qt::CaseInsensitive) == 0) {
            Playlist* p = playlists(i);
            playlists.removeAt(i); 
            delete p; 
            savePlaylists(); 
            return;
        }
    }
}

// THÊM HÀM ĐỂ SỬA LỖI BIÊN DỊCH LINKER
void MusicManager::renamePlaylist(const QString& oldName, const QString& newName) {
    Playlist* pl = getPlaylist(oldName);
    if (pl && getPlaylist(newName) == nullptr) {
        pl->setName(newName);
        savePlaylists();
    }
}

void MusicManager::savePlaylists() const {
    QFile file("playlists.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    for (int i = 0; i < playlists.getSize(); ++i) {
        Playlist* pl = playlists(i);
        if (pl->getIsTemporary()) continue;
        out << "PL:" << pl->getName() << "\n";
        const DoubleLinkedList<Song*>& sList = pl->getSongs();
        for (int j = 0; j < sList.getSize(); ++j) {
            out << "S:" << sList(j)->getTitle() << "|" << sList(j)->getArtist() << "\n";
        }
    }
    file.close();
}

void MusicManager::loadPlaylists() {
    QFile file("playlists.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    Playlist* currentPl = nullptr;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        if (line.startsWith("PL:")) {
            QString plName = line.mid(3); 
            currentPl = new Playlist(plName, false); 
            playlists.append(currentPl);
        } 
        else if (line.startsWith("S:") && currentPl) {
            QString content = line.mid(2);
            QStringList parts = content.split('|');
            if (parts.size() < 2) continue;
            QString title = parts[0];
            QString artist = parts[1];
            for (int k = 0; k < allSongs.getSize(); ++k) {
                if (allSongs(k)->getTitle() == title && allSongs(k)->getArtist() == artist) {
                    currentPl->addSong(allSongs(k));
                    break;
                }
            }
        }
    }
    file.close();
}

void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song) {
    if (!song) return;
    Playlist* targetPl = getPlaylist(playlistName);
    if (targetPl) {
        bool exists = false;
        const DoubleLinkedList<Song*>& currentSongs = targetPl->getSongs();
        for (int i = 0; i < currentSongs.getSize(); ++i) {
            if (currentSongs(i) == song) { exists = true; break; }
        }
        if (!exists) {
            targetPl->addSong(song);
            savePlaylists(); 
        }
    }
}

void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName) {
    Playlist* targetPl = getPlaylist(playlistName);
    if (!targetPl) return;
    DoubleLinkedList<Song*>& currentSongs = const_cast<DoubleLinkedList<Song*>&>(targetPl->getSongs());
    for (int i = 0; i < currentSongs.getSize(); ++i) {
        Song* s = currentSongs(i);
        if (s->getTitle() == songTitle && s->getArtist() == artistName) {
            currentSongs.removeAt(i); 
            savePlaylists();
            return;
        }
    }
}
DoubleLinkedList<Song*> MusicManager::searchHomeSong(const QString& word) const {
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < allSongs.getSize(); ++i) {
        // Tìm kiếm theo tên hoặc nghệ sĩ (không phân biệt hoa thường)
        if(allSongs(i)->getTitle().contains(word, Qt::CaseInsensitive) || 
           allSongs(i)->getArtist().contains(word, Qt::CaseInsensitive)) {
            res.append(allSongs(i));
        }
    }
    return res;
}

DoubleLinkedList<Song*> MusicManager::getTopSongs(int count) const {
    // Sửa dấu < thành > để bài nghe nhiều nhất lên đầu
    DoubleLinkedList<Song*> sorted = sortDLL(allSongs, [](const Song* a, const Song* b) { return a->getPlayCount() < b->getPlayCount(); });
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < std::min(count, sorted.getSize()); ++i) res.append(sorted(i));
    return res;
}

DoubleLinkedList<Song*> MusicManager::getNewReleases(int count) const {
    // Sửa dấu < thành > để ngày mới nhất lên đầu
    DoubleLinkedList<Song*> sorted = sortDLL(allSongs, [](const Song* a, const Song* b) { return a->getDateAdded() < b->getDateAdded(); });
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < std::min(count, sorted.getSize()); ++i) res.append(sorted(i));
    return res;
}

DoubleLinkedList<Song*> MusicManager::getSongsByArtist(const QString& name) const {
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < allSongs.getSize(); ++i) if(allSongs(i)->getArtist().compare(name, Qt::CaseInsensitive) == 0) res.append(allSongs(i));
    return res;
}

DoubleLinkedList<Song*> MusicManager::getRecommendedSongs(int count) const {
    DoubleLinkedList<Song*> res, temp = allSongs;
    int limit = std::min(count, temp.getSize());
    for(int i = 0; i < limit; ++i) {
        int idx = QRandomGenerator::global()->bounded(temp.getSize());
        res.append(temp(idx));
        temp.removeAt(idx);
    }
    return res;
}
DoubleLinkedList<Song*> MusicManager::getSongsByMood(const QString& moodName) const {
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < allSongs.getSize(); ++i) {
        // Kiểm tra nếu đường dẫn file chứa tên thư mục tâm trạng (vd: "CHILL")
        if(allSongs(i)->getFilePath().contains(moodName, Qt::CaseInsensitive)) {
            res.append(allSongs(i));
        }
    }
    return res;
}
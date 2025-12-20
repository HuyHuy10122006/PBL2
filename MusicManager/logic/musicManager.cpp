#include "musicManager.h"
#include <QRandomGenerator> 
#include <QFile> 
#include <QTextStream> 
#include <QDate> 
#include <functional> 

// Hàm bổ trợ sắp xếp
DoubleLinkedList<Song*> sortDLL(const DoubleLinkedList<Song*>& original, std::function<bool(const Song*, const Song*)> comparator) {
    DoubleLinkedList<Song*> sorted = original; 
    int n = sorted.getSize();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (comparator(sorted(j), sorted(j + 1))) {
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&file);
    out << "Title,Artist,FilePath,CoverPath,PlayCount,DateAdded\n"; // Cập nhật Header

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

void MusicManager::loadData() {
    QFile file("catalog.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    in.readLine(); // Bỏ qua header
    
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

MusicManager::MusicManager() : player(nullptr) { loadData(); }

MusicManager::~MusicManager() {
    saveData();
    if(player) {
        player->stop();
        // Không delete player nếu player đang tham chiếu tới các playlist sẽ bị delete ở dưới
        delete player; 
        player = nullptr;
    }
    // Xóa playlist trước, sau đó mới xóa bài hát
    for(int i = 0; i < playlists.getSize(); ++i) delete playlists(i);
    for(int i = 0; i < allSongs.getSize(); ++i) delete allSongs(i);
}

void MusicManager::playSongByObject(Song* s) {
    if (!s) return;

    if (!player) {
        player = new MusicPlayer();
    }

    // 1. Dừng ngay lập tức để ngắt kết nối với file cũ
    player->stop();
    player->setSource(QUrl()); // Xóa nguồn phát cũ khỏi bộ nhớ đệm

    // 2. Tạo một Playlist hoàn toàn mới với ID duy nhất (dùng thời gian để định danh)
    // Việc này giúp MusicPlayer không bao giờ dùng lại vùng nhớ vừa bị dừng
    QString uniqueName = "Single_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    Playlist* newTmp = new Playlist(uniqueName, true);
    newTmp->addSong(s);

    // 3. Nạp vào trình phát
    player->addPlist(newTmp);
    player->setAPlist(uniqueName);
    
    // 4. Phát
    player->play(0);

    // 5. Quản lý bộ nhớ: Lưu vào danh sách để destructor của Manager xóa sau này
    playlists.append(newTmp);

    s->setPlayCount(s->getPlayCount() + 1);
    qDebug() << "Success: Dang phat" << s->getTitle();
}
// Các hàm khác giữ nguyên logic cũ của bạn
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

void MusicManager::addPlaylist(Playlist* p) { if(p) playlists.append(p); }
void MusicManager::removePlaylist(const QString& name) {
    for(int i = 0; i < playlists.getSize(); ++i) {
        if(playlists(i)->getName() == name) {
            Playlist* p = playlists(i);
            playlists.removeAt(i);
            delete p;
            return;
        }
    }
}

DoubleLinkedList<Song*> MusicManager::getTopSongs(int count) const {
    DoubleLinkedList<Song*> sorted = sortDLL(allSongs, [](const Song* a, const Song* b) { return a->getPlayCount() < b->getPlayCount(); });
    DoubleLinkedList<Song*> res;
    for(int i = 0; i < std::min(count, sorted.getSize()); ++i) res.append(sorted(i));
    return res;
}

DoubleLinkedList<Song*> MusicManager::getNewReleases(int count) const {
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
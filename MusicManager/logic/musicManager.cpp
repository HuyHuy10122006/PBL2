#include "musicManager.h"
#include <QRandomGenerator> 
#include <QFile> 
#include <QTextStream> 
#include <QDate>
#include <functional>
#include <QFileInfo>

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

MusicManager::MusicManager() : player(nullptr) { 
    loadData();
    loadPlaylists(); 
}

MusicManager::~MusicManager() {
    //  Lưu dữ liệu trước khi xóa đối tượng khỏi bộ nhớ
    saveData();       // Lưu kho nhạc gốc
    savePlaylists();  // Lưu các playlist (hàm này đã có bộ lọc isTemporary)
    if(player) {
        player->stop();
        delete player; 
        player = nullptr;
    }
    for(int i = 0; i < playlists.getSize(); ++i) {
        delete playlists(i); 
    }
    for(int i = 0; i < allSongs.getSize(); ++i) {
        delete allSongs(i);
    }
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
DoubleLinkedList<Playlist*>& MusicManager::getPlaylists() {
    return playlists;
}

void MusicManager::addPlaylist(Playlist* p) { if(p) playlists.append(p); }
void MusicManager::removePlaylist(const QString& name) {
    for (int i = 0; i < playlists.getSize(); ++i) {
        // Sử dụng so sánh không phân biệt hoa thường để an toàn
        if (playlists(i)->getName().compare(name, Qt::CaseInsensitive) == 0) {
            Playlist* p = playlists(i);
            playlists.removeAt(i); // Sử dụng đúng tên hàm removeAt trong DoubleLinkedList của bạn
            delete p; // Giải phóng bộ nhớ đối tượng Playlist
            
            savePlaylists(); // QUAN TRỌNG: Lưu lại file sau khi xóa để cập nhật dữ liệu
            qDebug() << "Da xoa playlist:" << name;
            return;
        }
    }
}
void MusicManager::savePlaylists() const {
    QFile file("playlists.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Khong the mo file de ghi playlist!";
        return;
    }
    qDebug() << "DANG LUU PLAYLIST TAI:" << QFileInfo(file).absoluteFilePath();

    QTextStream out(&file);
    for (int i = 0; i < playlists.getSize(); ++i) {
        Playlist* pl = playlists(i);

        // ĐIỀU KIỆN QUAN TRỌNG: Chỉ lưu playlist người dùng tạo
        // Nếu pl->getIsTemporary() là true (Single_...), chúng ta bỏ qua
        if (pl->getIsTemporary()) continue;

        // Ghi tên Playlist với tiền tố PL:
        out << "PL:" << pl->getName() << "\n";
        
        // Ghi danh sách bài hát trong playlist đó
        const DoubleLinkedList<Song*>& sList = pl->getSongs();
        for (int j = 0; j < sList.getSize(); ++j) {
            // Lưu theo định dạng S:TênBài|TênCaSĩ để đối chiếu khi load
            out << "S:" << sList(j)->getTitle() << "|" << sList(j)->getArtist() << "\n";
        }
    }
    file.close();
    qDebug() << "Da luu cac playlist nguoi dung vao file playlists.csv";
}
void MusicManager::loadPlaylists() {
    QFile file("playlists.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Chua co file playlist hoac khong the mo.";
        return;
    }

    QTextStream in(&file);
    Playlist* currentPl = nullptr;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        if (line.startsWith("PL:")) {
            // Bước 1: Tạo đối tượng Playlist mới từ tên trong file
            QString plName = line.mid(3); 
            currentPl = new Playlist(plName, false); // false vì đây là playlist lưu trữ lâu dài
            playlists.append(currentPl);
        } 
        else if (line.startsWith("S:") && currentPl) {
            // Bước 2: Tách lấy Tên bài và Ca sĩ
            QString content = line.mid(2);
            QStringList parts = content.split('|');
            if (parts.size() < 2) continue;

            QString title = parts[0];
            QString artist = parts[1];

            // Bước 3: Tìm con trỏ bài hát tương ứng trong kho nhạc tổng (allSongs)
            // Việc này giúp tiết kiệm bộ nhớ (không tạo bài hát trùng lặp)
            for (int k = 0; k < allSongs.getSize(); ++k) {
                if (allSongs(k)->getTitle() == title && 
                    allSongs(k)->getArtist() == artist) {
                    currentPl->addSong(allSongs(k));
                    break;
                }
            }
        }
    }
    file.close();
    qDebug() << "Da nap xong danh sach playlist tu file.";
}
void MusicManager::addSongToPlaylist(const QString& playlistName, Song* song) {
    if (!song) return;

    // 1. Tìm Playlist mục tiêu theo tên
    Playlist* targetPl = getPlaylist(playlistName);

    if (targetPl) {
        // 2. Kiểm tra xem bài hát đã có trong Playlist chưa (tránh trùng lặp)
        bool exists = false;
        const DoubleLinkedList<Song*>& currentSongs = targetPl->getSongs();
        for (int i = 0; i < currentSongs.getSize(); ++i) {
            if (currentSongs(i) == song) {
                exists = true;
                break;
            }
        }

        // 3. Nếu chưa có thì thêm vào và lưu file ngay lập tức
        if (!exists) {
            targetPl->addSong(song);
            
            // QUAN TRỌNG: Gọi hàm lưu để cập nhật file playlists.csv ngay
            savePlaylists(); 
            
            qDebug() << "Success: Da them" << song->getTitle() << "vao playlist" << playlistName;
        } else {
            qDebug() << "Info: Bai hat da ton tai trong playlist nay.";
        }
    } else {
        qDebug() << "Error: Khong tim thay playlist ten là" << playlistName;
    }
}

void MusicManager::removeSongFromPlaylist(const QString& playlistName, const QString& songTitle, const QString& artistName) {
    // 1. Tìm Playlist mục tiêu
    Playlist* targetPl = getPlaylist(playlistName);
    if (!targetPl) return;

    // 2. Lấy danh sách bài hát (ép kiểu để có thể sửa đổi)
    DoubleLinkedList<Song*>& currentSongs = const_cast<DoubleLinkedList<Song*>&>(targetPl->getSongs());
    
    // 3. Duyệt danh sách bài hát trong playlist để tìm bài khớp thông tin
    for (int i = 0; i < currentSongs.getSize(); ++i) {
        Song* s = currentSongs(i);
        if (s->getTitle() == songTitle && s->getArtist() == artistName) {
            currentSongs.removeAt(i); // Xóa khỏi danh sách liên kết
            savePlaylists();          // Cập nhật lại file playlists.csv ngay lập tức
            qDebug() << "Da xoa bài hat:" << songTitle << "khoi playlist:" << playlistName;
            return;
        }
    }
}DoubleLinkedList<Song*> MusicManager::getTopSongs(int count) const {
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

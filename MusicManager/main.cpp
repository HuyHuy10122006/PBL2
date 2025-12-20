#include "login.h"
#include "logic/musicManager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Khởi tạo manager
    MusicManager *manager = new MusicManager();

    // Truyền manager vào login
    login w(manager);
    w.show();

    return a.exec();
}

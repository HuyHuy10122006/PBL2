#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include "logic/musicManager.h" // Cần thiết để nhận diện kiểu dữ liệu

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    // Sửa constructor để nhận MusicManager
    explicit login(MusicManager *manager, QWidget *parent = nullptr);
    ~login();

private:
    Ui::login *ui;
    MusicManager *m_manager; // Biến để lưu trữ manager được truyền vào
};

#endif // LOGIN_H

#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <QString>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QTextStream>
#include <QDebug> 

class AuthService{
private:
    static const QString ACCOUNT_FILE; // Tên file lưu trữ tài khoản
    static QByteArray hashPassword(const QString& password) // Hàm băm mật khẩu
    {
        return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    }
    // Hàm kiểm tra tồn tại (chỉ tìm username)
    static bool isUserExist(const QString& username)
    {
        QFile file(ACCOUNT_FILE); // Mở file
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false; // Nếu không mở được, trả về false
        QTextStream in(&file); // Tạo luồng đọc file
        QString user, storedHash; // Biến tạm lưu trữ dữ liệu
        // Đọc 2 giá trị (user và hash)
        while (!in.atEnd()) {
            in >> user >> storedHash;

            if (user == username)
                return true;
        }
        return false;
    }

public:
    // ĐĂNG KÝ: Chỉ lưu trữ Hash
    static bool registerUser(const QString& username, const QString& password)
    {
        if(isUserExist(username)) {
            qDebug() << "username đã tồn tại.";
            return false;
        }
        // 1. Tạo Hash
        QByteArray hashedPassword = hashPassword(password);
        QFile file(ACCOUNT_FILE); // Mở file để ghi
        if (!file.open(QIODevice::Append | QIODevice::Text)) return false; // Nếu không mở được, trả về false
        QTextStream out(&file);// Tạo luồng ghi file
        // Lưu trữ: username, hash
        out << username << " " << hashedPassword << "\n";
        qDebug() << "User registered:" << username;
        return true;
    }

    // ĐĂNG NHẬP: Tái tạo Hash để so sánh
    static bool loginUser(const QString& username, const QString& password)
    {
        QFile file(ACCOUNT_FILE);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
        QTextStream in(&file);// Tạo luồng đọc file
        
        QString user, storedHash;
        
        // Vòng lặp đọc 2 giá trị (user và hash)
        while (!in.atEnd()) {
            in >> user >> storedHash;

            if (user == username) {
                // 1. Tái tạo Hash từ mật khẩu người dùng nhập vào
                QByteArray enteredHash = hashPassword(password);

                // 2. So sánh Hash
                return enteredHash == storedHash.toUtf8();
            }
        }
        qDebug() << "Login failed: User or password incorrect.";
        return false;
    }
};
const QString AuthService::ACCOUNT_FILE = "account.txt";
#endif

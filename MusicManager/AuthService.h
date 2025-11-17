#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H

#include <string>
#include <fstream>
using namespace std;

class AuthService{
    static bool isUserExist(const string& username)
    {
        ifstream file("account.txt");
        if(!file.is_open()) return false;
        string user, pass;
        while(file >> user >> pass)
        {
            if(user == username){
                file.close();
                return true; //đã tồn tại
            }
        }
        file.close();
        return false;
    }
    public:
    static bool registerUser(const string& username, const string& password)
    {
        if(isUserExist(username)) return false;
        ofstream file("account.txt",ios::app);// ghi nối vào cuối file
        if(!file.is_open()) return false;
        file << username << " " << password << "\n";
        file.close();
        return true;
    }
    static bool loginUser(const string& username, const string& password)
    {
        ifstream file("account.txt");
        if(!file.is_open()) return false;
        string user, pass;
        while(file >> user >> pass)
        {
            if(user == username && pass == password){
                file.close();
                return true;
        }
        }
        file.close();
        return false; 
    }
};
#endif
#ifndef HOME_H
#define HOME_H

#include <QWidget>
#include <QString>

namespace Ui {
class Home;
}

class Home : public QWidget
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void showArtistDetail(const QString &artistName);
    void showMoodDetail(const QString &moodName);

private:
    Ui::Home *ui;
};

#endif

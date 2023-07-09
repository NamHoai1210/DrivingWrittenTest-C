#ifndef REGISTER_H
#define REGISTER_H
#include "../rapidjson/document.h"
#include <QMainWindow>
#include "../Entity/User/user.h"
#include <QDebug>
#include <QLabel>
#include "../connect/connect.h"
#include "../session/session.h"
QT_BEGIN_NAMESPACE
namespace  Ui {
class AuthWindow;
}
QT_END_NAMESPACE

class AuthWindow: public QMainWindow{
    Q_OBJECT

public:
    AuthWindow(QWidget *parent = nullptr);
    ~AuthWindow();
private slots:
    void on_signIn_5_clicked();

    void on_signIn_4_clicked();

private:
    Ui::AuthWindow *ui;
    bool readResponse(char*, QLabel*);
    User* user;
signals:
    void success(std::string);
};

#endif // REGISTER_H

#ifndef REGISTER_H
#define REGISTER_H

#include <QMainWindow>
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
private:
    Ui::AuthWindow *ui;
};

#endif // REGISTER_H

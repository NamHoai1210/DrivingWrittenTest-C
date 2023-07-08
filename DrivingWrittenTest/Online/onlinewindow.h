#ifndef ONLINEWINDOW_H
#define ONLINEWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "../Room/room.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class OnlineWindow;
}
QT_END_NAMESPACE
class OnlineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OnlineWindow(QMainWindow *parent = nullptr);
    ~OnlineWindow();

private slots:
    void on_createRoom_3_clicked();

private:
    Ui::OnlineWindow *ui;
    QMainWindow* parent;
};

#endif // ONLINEWINDOW_H

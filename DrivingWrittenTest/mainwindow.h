#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "./Practice/practice.h"
#include "./Online/onlinewindow.h"
#include "./session/session.h"
#include "./Auth/auth.h"
#include "../connect/connect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSuccess(std::string);

    void on_practice_clicked();

    void on_online_clicked();

    void on_signout_clicked();

private:
    Ui::MainWindow *ui;
    PracticeWindow* practiceWindow;
    OnlineWindow* onlineWindow;
    AuthWindow* authWindow;
};
#endif // MAINWINDOW_H

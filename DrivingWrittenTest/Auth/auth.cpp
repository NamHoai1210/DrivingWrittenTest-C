#include "auth.h"
#include "./ui_auth.h"
AuthWindow::AuthWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    QPushButton *signInNav = ui->signInNav;
    connect(signInNav, &QPushButton::clicked, this, [=]() {
        //set page to 1
        ui->stackedWidget->setCurrentIndex(0);
    });
    QPushButton *signupNav = ui->signupNav;
    connect(signupNav, &QPushButton::clicked, this, [=]() {
        //set page to 1
        ui->stackedWidget->setCurrentIndex(1);
    });
}

AuthWindow::~AuthWindow()
{
    delete ui;
}

#include "auth.h"
#include "./ui_auth.h"
AuthWindow::AuthWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AuthWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    this->user= new User();
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

bool AuthWindow::readResponse(char* res, QLabel* error){
    rapidjson::Document document;
    document.Parse(res);
    if (document.HasMember("status") && document["status"].IsString()) {
        const std::string status = document["status"].GetString();
        if(status == "success"){
            error->setText("");
            return true;
        }else{
            if (document.HasMember("message") && document["message"].IsString()) {
                const std::string message = document["message"].GetString();
                error->setText(QString::fromStdString(message));
                return false;
            }
        }
    }
    ui->SIerror->setText("Unknow error");
    return false;
}

void AuthWindow::on_signIn_5_clicked()
{
    QString tmp_usename = ui->SIusernameEntry->text();
    QString tmp_password = ui->SIpasswordEntry->text();
    this->user->setUsername(tmp_usename.toStdString());
    this->user->setPassword(tmp_password.toStdString());
    std::string str0(user->toJsonString("login"));
    qDebug() << QString::fromStdString(str0);
    char* res = Connect::getInstance()->both(user->toJsonString("login"));
    std::string str(res);
    qDebug() << QString::fromStdString(str);
    bool check = readResponse(res, ui->SIerror);
    if(check){
        qDebug() << "true";
        delete this->user;
        emit success(tmp_usename.toStdString());
    }

}


void AuthWindow::on_signIn_4_clicked()
{
    QString tmp_usename = ui->SUusernameEntry->text();
    QString tmp_password = ui->SUpasswordEntry->text();
    QString tmp_cpassword = ui->SUcpasswordEntry->text();
    if(tmp_password != tmp_cpassword){
        ui->SUerror->setText("Confirm password doesnt match!");
    }else{
    this->user->setUsername(tmp_usename.toStdString());
    this->user->setPassword(tmp_password.toStdString());
    char* res = Connect::getInstance()->both(user->toJsonString("signup"));
    bool check = readResponse(res, ui->SUerror);
    if(check){
        qDebug() << "true";
        ui->SUerror->setText("Successful! Please return the Sign In screen to continue!");
        ui->SUerror->setStyleSheet(QString("color: green;"));
    }
    }
}


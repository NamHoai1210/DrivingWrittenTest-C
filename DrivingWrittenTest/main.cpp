#include "mainwindow.h"
#include "Auth/auth.h"
#include "Practice/practice.h"
#include "Online/onlinewindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "DrivingWrittenTest_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
//    AuthWindow r;
//    r.show();
//    PracticeWindow p;
//    p.show();
//    OnlineWindow o;
//    o.show();

    return a.exec();
}

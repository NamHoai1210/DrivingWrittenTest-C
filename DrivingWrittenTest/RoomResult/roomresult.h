#ifndef ROOMRESULT_H
#define ROOMRESULT_H

#include <QDialog>
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/writer.h"
#include "../session/session.h"
#include "../Utils/utils.h"
#include "../connect/connect.h"
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
namespace Ui {
class RoomResult;
}

class RoomResult : public QDialog
{
    Q_OBJECT

public:
    explicit RoomResult(QWidget *parent = nullptr);
    explicit RoomResult(QWidget *parent = nullptr, std::string roomId = "", std::string level = "EASY", std::string start="");
    ~RoomResult();

private:
    Ui::RoomResult *ui;
};

#endif // ROOMRESULT_H

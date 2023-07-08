#ifndef UTILS_H
#define UTILS_H
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include <iostream>
#include <QFile>
#include <QResource>
#include <QDebug>
#include <QTextStream>
#include "../Entity/History/history.h"
bool readJSONFromResource(const QString& resourcePath, rapidjson::Document& document);
std::vector<History*> readHistory();
void writeHistory(std::vector<History*>);
#endif // UTILS_H
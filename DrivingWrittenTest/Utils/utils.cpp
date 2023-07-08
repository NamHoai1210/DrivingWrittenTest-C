#include "utils.h"

QString basePath = "/home/namhoai/Workspace/DrivingWrittenTest-C/DrivingWrittenTest";

bool readJSONFromResource(const QString& resourcePath, rapidjson::Document& document)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        std::cout << "Failed to open the JSON file." << std::endl;
        return false;
    }

    QTextStream stream(&file);
    QString jsonString = stream.readAll().toUtf8();

    file.close();

    rapidjson::Document tempDocument;
    tempDocument.Parse(jsonString.toStdString().c_str());

    if (tempDocument.HasParseError())
    {
        std::cout << "Failed to parse the JSON file. Error code: " << tempDocument.GetParseError() << std::endl;
        return false;
    }

    document = std::move(tempDocument);
    return true;
}

std::vector<History*> readHistory() {

    // Define the file path
    QString filePath = basePath + "/db/history.txt";  // Assuming the file is located in the resource directory
    std::vector<History*> history;
    // Open the file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file for reading:" << file.errorString();
        return history;
    }

    // Read the file content
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();

        // Split the line by colon
        QStringList parts = line.split(',');
        if (parts.size() == 4) {
            QString time = parts[0].trimmed();
            QString level = parts[1].trimmed();
            QString duration = parts[2].trimmed();
            QString score = parts[3].trimmed();
            history.push_back(new History(time.toStdString(),level.toStdString(),duration.toStdString(),score.toFloat()));
        }
    }

    // Close the file
    file.close();

    return history;
}

void writeHistory(std::vector<History*> history) {

    // Define the file path
    QString filePath = basePath + "db/history.txt";  // Assuming the file is located in the resource directory

    // Open the file for writing
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << file.errorString();
        return;
    }
    // Create a text stream for writing
    QTextStream out(&file);
    for(int i=0;i< history.size();i++ ){
        out << QString::fromStdString(history[i]->getTime()) << "," << QString::fromStdString(history[i]->getLevel()) << "," << QString::fromStdString(history[i]->getDuration()) << "," << QString::number(history[i]->getScore()) << "\n";
    }
    // Close the file
    file.close();
}

rapidjson::Value tojsonValueType(std::string name,rapidjson::Document::AllocatorType& allocator){
    rapidjson::Value result(name.c_str(), static_cast<rapidjson::SizeType>(name.length()), allocator);
    return result;
}

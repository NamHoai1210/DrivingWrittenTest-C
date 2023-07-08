#ifndef QUESTION_H
#define QUESTION_H
#include <vector>
#include <string>
#include <QString>
#include "../../rapidjson/document.h"
#include "../../rapidjson/filereadstream.h"

class Question
{
private:
    std::string question;
    std::vector<std::string> answers;
    int answer = -1;

public:
    Question();
    Question(std::string);
    Question(std::string, std::vector<std::string>);
    void setQuestion(std::string);
    std::string getQuestion();
    void addAnswer(std::string);
    std::string getAnswer(int);
    std::vector<std::string> getAnswers();
    int getTrue();
    void setTrue(int);
    static void readQuestionFile(std::vector<Question*>&,const QString);
};

#endif // QUESTION_H

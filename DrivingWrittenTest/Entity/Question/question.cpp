#include "question.h"
#include <QDebug>
#include <QFile>
#include "../Utils/utils.h"
Question::Question()
{
    this->answer = -1;
}

Question::Question(std::string q)
{
    this->answer = -1;
    this->question = q;
}

Question::Question(std::string q, std::vector<std::string> a)
{
    this->answer = -1;
    this->question = q;
    this->answers = a;
}

void Question::addAnswer(std::string a){
    this->answers.push_back(a);
}

std::string Question::getAnswer(int i){
    return this->answers[i];
}

void Question::setQuestion(std::string q){
    this->question = q;
}

std::string Question::getQuestion(){
    return this->question;
}

std::vector<std::string> Question::getAnswers(){
    return this->answers;
}

void Question::setTrue(int i){
    this->answer = i;
}

int Question::getTrue(){
    return this->answer;
}

void Question::readQuestionFile(std::vector<Question*>& qList, const QString resourcePath){
    rapidjson::Document document;

    if (readJSONFromResource(resourcePath, document))
    {
        // Check if the parsed document is an array
        if (document.IsArray())
        {
            // Iterate over the array elements
            for (rapidjson::SizeType i = 0; i < document.Size(); ++i)
            {
                const rapidjson::Value& obj = document[i];
                Question * q = new Question();

                // Access the object properties
                if (obj.HasMember("question") && obj["question"].IsString())
                {
                   const char* question =  obj["question"].GetString();
                    q->setQuestion(question);

                }
                if (obj.HasMember("answers") && obj["answers"].IsArray())
                {
                    rapidjson::Value::ConstArray a = obj["answers"].GetArray();
                    for(rapidjson::SizeType i=0;i< a.Size();i++){
                        if (a[i].IsString())
                        {
                            q->addAnswer(a[i].GetString());
                        }
                    }

                }
                if (obj.HasMember("answer") && obj["answer"].IsInt()){
                    const int answer = obj["answer"].GetInt();
                    q->setTrue(answer);
                }
                qList.push_back(q);
                // Continue accessing other properties as needed
            }
        }
        else
        {
            qDebug() << "JSON file does not contain an array of objects." ;
        }
    }
}

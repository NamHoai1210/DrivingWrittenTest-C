#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <stdexcept>

using namespace std;

struct Question
{
    string questionText;
    vector<string> choices;
    int correctChoice;
};

vector<Question> loadQuestionsFromFile(const string &filename)
{
    vector<Question> questions;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Failed to open the file: " << filename << endl;
        return questions;
    }

    string line;
    while (getline(file, line))
    {
        Question question;
        question.questionText = line;

        for (int i = 0; i < 4; ++i)
        {
            getline(file, line);
            question.choices.push_back(line);
        }

        getline(file, line);
        if (line == "1" || line == "2" || line == "3" || line == "4")
        {
            question.correctChoice = stoi(line);
        }
        else
        {
            cout << "Invalid correct choice: " << line << endl;
            file.close();
            return questions;
        }

        questions.push_back(question);
    }

    file.close();
    return questions;
}

int main()
{
    string filename = "questions.txt"; // Replace with your own filename

    vector<Question> questions = loadQuestionsFromFile(filename);
    if (questions.empty())
    {
        return 1;
    }

    int numQuestions = questions.size();
    int timeLimit = 120; // Time limit in seconds

    cout << "Welcome to the Practice Test!" << endl;
    cout << "You will have " << timeLimit << " seconds to answer " << numQuestions << " questions." << endl;
    cout << "Press any key to start the test..." << endl;
    cin.get();

    srand(time(0)); // Seed the random number generator

    int numCorrect = 0;
    time_t startTime = time(0);

    for (int i = 0; i < numQuestions; ++i)
    {
        // system("cls"); // Clear the console (for Windows)

        cout << "Question " << (i + 1) << "/" << numQuestions << ":" << endl;
        cout << questions[i].questionText << endl;

        for (int j = 0; j < 4; ++j)
        {
            cout << (j + 1) << ". " << questions[i].choices[j] << endl;
        }

        int userChoice;
        cout << "Your choice: ";
        cin >> userChoice;

        if (userChoice == questions[i].correctChoice)
        {
            numCorrect++;
        }
    }

    time_t endTime = time(0);
    int elapsedTime = endTime - startTime;

    system("cls");
    cout << "Test completed!" << endl;
    cout << "Number of correct answers: " << numCorrect << endl;
    cout << "Time taken: " << elapsedTime << " seconds." << endl;

    return 0;
}

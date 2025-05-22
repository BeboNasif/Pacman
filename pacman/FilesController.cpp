#include "FilesController.h"
#include <fstream>




void FilesController::writeFile(const string& filename, const vector<string>& content) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& line : content) {
            file << line << endl;
        }
        file.close();
    } else {
        cout << "Error opening file for writing: " << filename << endl;
    }
}
vector<string> FilesController::readFile(const string& filename) {
    vector<string> content;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            content.push_back(line);
        }
        file.close();
    } else {
        cout << "Error opening file for reading: " << filename << endl;
    }
    return content;
}

map<string, map<string, string>> FilesController::getAllSettings() {
    vector<string> lines = readFile("Assets/Settings.txt");
    map<string, map<string, string>> allsettings;
    for(string line : lines){
        string key,value, curName;

        int i = 0;
        while(line[i]!=':'){
            curName += line[i];
            i++;
        }
        i++;
        bool isKey = true;
        while(i < line.size() and line[i]!=';'){
            if(isKey){
                while(i < line.size() and line[i]!='='){
                    key += line[i];
                    i++;
                }
                i++;
                isKey = false;
            }else {
                while(i < line.size() and (line[i]!=',' and line[i]!=';')){
                    value += line[i];
                    i++;
                }
                i++;
                allsettings[curName][key] = value;
                key.clear();
                value.clear();
                isKey = true;
            }
        }
    }
    return allsettings;
}
map<string, string> FilesController::getPlayerSettings(const string playername) {
    map<string, map<string, string>> allsettings = FilesController::getAllSettings();
    return allsettings[playername];
}

string FilesController::parsePlayerSettingsToText(const map<string, string> settings,const string playerName) {
    string result=playerName+":";
    for (const auto& pair : settings) {
        result += pair.first + "=" + pair.second + ",";
    }
    return result+';';
}

void FilesController::updatePlayerSettings(const string playerName, const map<string, string>& settings) {
    map<string, map<string, string>> allsettings = FilesController::getAllSettings();
    allsettings[playerName] = settings;
    vector<string> lines;
    for (const auto& pair : allsettings) {
        lines.push_back(parsePlayerSettingsToText(pair.second,pair.first));
    }
    FilesController::writeFile("Assets/Settings.txt", lines);
}


void FilesController::Gameover(int curscore,int maxscore) {
    highscoreENDtx.loadFromFile("Assets/Textures/highscore.png");
    highscoreENDsp.setTexture(highscoreENDtx);
    highscoreENDsp.setPosition(500 , 5);
    highscoreENDsp.setScale(1.7, 1.7);

    overlay.setSize(Vector2f(1920, 1080));
    overlay.setFillColor(Color(0, 0, 0, 100));

    winnertx.loadFromFile("Assets/Textures/winner.png");
    winnersp.setTexture(winnertx);
    winnersp.setPosition(550, 400);
    winnersp.setScale(1.7, 1.7);


    gameovertx.loadFromFile("Assets/Textures/gameover0.png");
    gameoversp.setTexture(gameovertx);
    gameoversp.setPosition(500 , 10);
    gameoversp.setScale(1.8, 1.8),

    font.loadFromFile("Assets/Fonts/BrownieStencil-8O8MJ.ttf");
    scoreText1.setFont(font);
    scoreText2.setFont(font);


    scoreText1.setString("SCORE:     " + to_string(curscore));
    scoreText2.setString("MAX SCORE:  " + to_string(maxscore));

    //scoreText3.setString()
    scoreText1.setPosition(850 , 600);
    scoreText2.setPosition(850 , 700);

    scoreText1.setCharacterSize(60);
    scoreText2.setCharacterSize(60);

    scoreText1.setFillColor(Color::White);
    scoreText2.setFillColor(Color::White);


}



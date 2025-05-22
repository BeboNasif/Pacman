#include "include.h"

class FilesController {
public:
    static void writeFile(const string& filename, const vector<string>& content);
    static vector<string> readFile(const string& filename);
    static map<string, map<string, string>> getAllSettings();
    static void updatePlayerSettings(const string playerName, const map<string, string>& settings);
    static string parsePlayerSettingsToText(const map<string, string> settings,const string playerName);
    static map<string, string> getPlayerSettings(const string playername);
    void Gameover(int curscore,int maxscore);
    Texture highscoreENDtx, gameovertx, winnertx;
    Sprite highscoreENDsp, gameoversp, winnersp;
    Font font;
    Text scoreText1,scoreText2;
    RectangleShape overlay;
};
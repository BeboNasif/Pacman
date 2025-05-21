#include "FilesController.h"
#include <fstream>

void FilesController::writeFile(const string& filename, const vector<string>& content) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& line : content) {
            file << line << endl;
        }
        file.close();
    }
    else {
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
    }
    else {
        cout << "Error opening file for reading: " << filename << endl;
    }
    return content;
}

map<string, map<string, string>> FilesController::getAllSettings() {
    vector<string> lines = readFile("Settings.txt");
    cerr << "file read" << endl;
    map<string, map<string, string>> allsettings;
    for (string line : lines) {
        cerr << "processing line: " << line << endl;
        string key, value, curName;

        int i = 0;
        while (line[i] != ':') {
            curName += line[i];
            i++;
        }
        i++;
        bool isKey = true;
        while (i < line.size() and line[i] != ';') {
            if (isKey) {
                while (i < line.size() and line[i] != '=') {
                    key += line[i];
                    i++;
                }
                i++;
                isKey = false;
            }
            else {
                while (i < line.size() and (line[i] != ',' and line[i] != ';')) {
                    value += line[i];
                    i++;
                }
                i++;
                cerr << "key: " << key << " value: " << value << endl;
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
    cerr << "loaded all settings" << endl;
    return allsettings[playername];
}

string FilesController::parsePlayerSettingsToText(const map<string, string> settings, const string playerName) {
    string result = playerName + ":";
    for (const auto& pair : settings) {
        result += pair.first + "=" + pair.second + ",";
    }
    return result + ';';
}

void FilesController::updatePlayerSettings(const string playerName, const map<string, string>& settings) {
    map<string, map<string, string>> allsettings = FilesController::getAllSettings();
    allsettings[playerName] = settings;
    vector<string> lines;
    for (const auto& pair : allsettings) {
        lines.push_back(parsePlayerSettingsToText(pair.second, pair.first));
    }
    FilesController::writeFile("Settings.txt", lines);
    cerr << "updated" << endl;
}
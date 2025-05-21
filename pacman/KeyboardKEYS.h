#include "include.h"

class KeyboardKEYS {
public:
    static string keyboardKeyToString(sf::Keyboard::Key key);
    static sf::Keyboard::Key KeyNameToKey(const string& keyName);
};
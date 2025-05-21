#include "KeyboardKEYS.h"

string KeyboardKEYS::keyboardKeyToString(sf::Keyboard::Key key) {
    switch (key) {
    case sf::Keyboard::A:      return "A";
    case sf::Keyboard::B:      return "B";
    case sf::Keyboard::C:      return "C";
    case sf::Keyboard::D:      return "D";
    case sf::Keyboard::E:      return "E";
    case sf::Keyboard::F:      return "F";
    case sf::Keyboard::G:      return "G";
    case sf::Keyboard::H:      return "H";
    case sf::Keyboard::I:      return "I";
    case sf::Keyboard::J:      return "J";
    case sf::Keyboard::K:      return "K";
    case sf::Keyboard::L:      return "L";
    case sf::Keyboard::M:      return "M";
    case sf::Keyboard::N:      return "N";
    case sf::Keyboard::O:      return "O";
    case sf::Keyboard::P:      return "P";
    case sf::Keyboard::Q:      return "Q";
    case sf::Keyboard::R:      return "R";
    case sf::Keyboard::S:      return "S";
    case sf::Keyboard::T:      return "T";
    case sf::Keyboard::U:      return "U";
    case sf::Keyboard::V:      return "V";
    case sf::Keyboard::W:      return "W";
    case sf::Keyboard::X:      return "X";
    case sf::Keyboard::Y:      return "Y";
    case sf::Keyboard::Z:      return "Z";
    case sf::Keyboard::Num0:   return "0";
    case sf::Keyboard::Num1:   return "1";
    case sf::Keyboard::Num2:   return "2";
    case sf::Keyboard::Num3:   return "3";
    case sf::Keyboard::Num4:   return "4";
    case sf::Keyboard::Num5:   return "5";
    case sf::Keyboard::Num6:   return "6";
    case sf::Keyboard::Num7:   return "7";
    case sf::Keyboard::Num8:   return "8";
    case sf::Keyboard::Num9:   return "9";
    case sf::Keyboard::Escape: return "Escape";
    case sf::Keyboard::Space:  return "Space";
    case sf::Keyboard::Return: return "------";
    case sf::Keyboard::Tab:    return "Tab";
    case sf::Keyboard::Left:   return "Left";
    case sf::Keyboard::Right:  return "Right";
    case sf::Keyboard::Up:     return "Up";
    case sf::Keyboard::Down:   return "Down";
    case sf::Keyboard::Numpad0:   return "Numpad 0";
    default:                    return "Unknown";
    }

    
}

sf::Keyboard::Key KeyboardKEYS::KeyNameToKey(const string& keyName) {
    if (keyName == "Left") return sf::Keyboard::Left;
    if (keyName == "Right") return sf::Keyboard::Right;
    if (keyName == "Up") return sf::Keyboard::Up;
    if (keyName == "Down") return sf::Keyboard::Down;
    if (keyName == "A") return sf::Keyboard::A;
    if (keyName == "B") return sf::Keyboard::B;
    if (keyName == "C") return sf::Keyboard::C;
    if (keyName == "D") return sf::Keyboard::D;
    if (keyName == "E") return sf::Keyboard::E;
    if (keyName == "F") return sf::Keyboard::F;
    if (keyName == "G") return sf::Keyboard::G;
    if (keyName == "H") return sf::Keyboard::H;
    if (keyName == "I") return sf::Keyboard::I;
    if (keyName == "J") return sf::Keyboard::J;
    if (keyName == "K") return sf::Keyboard::K;
    if (keyName == "L") return sf::Keyboard::L;
    if (keyName == "M") return sf::Keyboard::M;
    if (keyName == "N") return sf::Keyboard::N;
    if (keyName == "O") return sf::Keyboard::O;
    if (keyName == "P") return sf::Keyboard::P;
    if (keyName == "Q") return sf::Keyboard::Q;
    if (keyName == "R") return sf::Keyboard::R;
    if (keyName == "S") return sf::Keyboard::S;
    if (keyName == "T") return sf::Keyboard::T;
    if (keyName == "U") return sf::Keyboard::U;
    if (keyName == "V") return sf::Keyboard::V;
    if (keyName == "W") return sf::Keyboard::W;
    if (keyName == "X") return sf::Keyboard::X;
    if (keyName == "Y") return sf::Keyboard::Y;
    if (keyName == "Z") return sf::Keyboard::Z;
    if (keyName == "0") return sf::Keyboard::Num0;
    if (keyName == "1") return sf::Keyboard::Num1;
    if (keyName == "2") return sf::Keyboard::Num2;
    if (keyName == "3") return sf::Keyboard::Num3;
    if (keyName == "4") return sf::Keyboard::Num4;
    if (keyName == "5") return sf::Keyboard::Num5;
    if (keyName == "6") return sf::Keyboard::Num6;
    if (keyName == "7") return sf::Keyboard::Num7;
    if (keyName == "8") return sf::Keyboard::Num8;
    if (keyName == "9") return sf::Keyboard::Num9;
    if (keyName == "Escape") return sf::Keyboard::Escape;
    if (keyName == "Space") return sf::Keyboard::Space;
    if (keyName == "Return") return sf::Keyboard::Return;
    if (keyName == "Tab") return sf::Keyboard::Tab;
    if (keyName == "Numpad 0") return sf::Keyboard::Numpad0;
    if (keyName == "Numpad 1") return sf::Keyboard::Numpad1;
    if (keyName == "Numpad 2") return sf::Keyboard::Numpad2;
    if (keyName == "Numpad 3") return sf::Keyboard::Numpad3;
    if (keyName == "Numpad 4") return sf::Keyboard::Numpad4;
    if (keyName == "Numpad 5") return sf::Keyboard::Numpad5;
    if (keyName == "Numpad 6") return sf::Keyboard::Numpad6;
    if (keyName == "Numpad 7") return sf::Keyboard::Numpad7;
    if (keyName == "Numpad 8") return sf::Keyboard::Numpad8;
    if (keyName == "Numpad 9") return sf::Keyboard::Numpad9;
    if (keyName == "Left Shift") return sf::Keyboard::LShift;
    if (keyName == "Right Shift") return sf::Keyboard::RShift;
    if (keyName == "Left Control") return sf::Keyboard::LControl;
    if (keyName == "Right Control") return sf::Keyboard::RControl;
    if (keyName == "Left Alt") return sf::Keyboard::LAlt;
    if (keyName == "Right Alt") return sf::Keyboard::RAlt;
    
    return sf::Keyboard::Unknown; // Default case
}
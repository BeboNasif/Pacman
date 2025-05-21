#include "KeyboardKEYS.h"
#include "include.h"
string KeyboardKEYS::keyboardKeyToString(Keyboard::Key key) {
    switch (key) {
    case Keyboard::A:      return "A";
    case Keyboard::B:      return "B";
    case Keyboard::C:      return "C";
    case Keyboard::D:      return "D";
    case Keyboard::E:      return "E";
    case Keyboard::F:      return "F";
    case Keyboard::G:      return "G";
    case Keyboard::H:      return "H";
    case Keyboard::I:      return "I";
    case Keyboard::J:      return "J";
    case Keyboard::K:      return "K";
    case Keyboard::L:      return "L";
    case Keyboard::M:      return "M";
    case Keyboard::N:      return "N";
    case Keyboard::O:      return "O";
    case Keyboard::P:      return "P";
    case Keyboard::Q:      return "Q";
    case Keyboard::R:      return "R";
    case Keyboard::S:      return "S";
    case Keyboard::T:      return "T";
    case Keyboard::U:      return "U";
    case Keyboard::V:      return "V";
    case Keyboard::W:      return "W";
    case Keyboard::X:      return "X";
    case Keyboard::Y:      return "Y";
    case Keyboard::Z:      return "Z";
    case Keyboard::Num0:   return "0";
    case Keyboard::Num1:   return "1";
    case Keyboard::Num2:   return "2";
    case Keyboard::Num3:   return "3";
    case Keyboard::Num4:   return "4";
    case Keyboard::Num5:   return "5";
    case Keyboard::Num6:   return "6";
    case Keyboard::Num7:   return "7";
    case Keyboard::Num8:   return "8";
    case Keyboard::Num9:   return "9";
    case Keyboard::Escape: return "Escape";
    case Keyboard::Space:  return "Space";
    case Keyboard::Return: return "------";
    case Keyboard::Tab:    return "Tab";
    case Keyboard::Left:   return "Left";
    case Keyboard::Right:  return "Right";
    case Keyboard::Up:     return "Up";
    case Keyboard::Down:   return "Down";
    case Keyboard::Numpad0:   return "Numpad 0";
    default:                    return "Unknown";
    }

    
}

Keyboard::Key KeyboardKEYS::KeyNameToKey(const string& keyName) {
    if (keyName == "Left") return Keyboard::Left;
    if (keyName == "Right") return Keyboard::Right;
    if (keyName == "Up") return Keyboard::Up;
    if (keyName == "Down") return Keyboard::Down;
    if (keyName == "A") return Keyboard::A;
    if (keyName == "B") return Keyboard::B;
    if (keyName == "C") return Keyboard::C;
    if (keyName == "D") return Keyboard::D;
    if (keyName == "E") return Keyboard::E;
    if (keyName == "F") return Keyboard::F;
    if (keyName == "G") return Keyboard::G;
    if (keyName == "H") return Keyboard::H;
    if (keyName == "I") return Keyboard::I;
    if (keyName == "J") return Keyboard::J;
    if (keyName == "K") return Keyboard::K;
    if (keyName == "L") return Keyboard::L;
    if (keyName == "M") return Keyboard::M;
    if (keyName == "N") return Keyboard::N;
    if (keyName == "O") return Keyboard::O;
    if (keyName == "P") return Keyboard::P;
    if (keyName == "Q") return Keyboard::Q;
    if (keyName == "R") return Keyboard::R;
    if (keyName == "S") return Keyboard::S;
    if (keyName == "T") return Keyboard::T;
    if (keyName == "U") return Keyboard::U;
    if (keyName == "V") return Keyboard::V;
    if (keyName == "W") return Keyboard::W;
    if (keyName == "X") return Keyboard::X;
    if (keyName == "Y") return Keyboard::Y;
    if (keyName == "Z") return Keyboard::Z;
    if (keyName == "0") return Keyboard::Num0;
    if (keyName == "1") return Keyboard::Num1;
    if (keyName == "2") return Keyboard::Num2;
    if (keyName == "3") return Keyboard::Num3;
    if (keyName == "4") return Keyboard::Num4;
    if (keyName == "5") return Keyboard::Num5;
    if (keyName == "6") return Keyboard::Num6;
    if (keyName == "7") return Keyboard::Num7;
    if (keyName == "8") return Keyboard::Num8;
    if (keyName == "9") return Keyboard::Num9;
    if (keyName == "Escape") return Keyboard::Escape;
    if (keyName == "Space") return Keyboard::Space;
    if (keyName == "Return") return Keyboard::Return;
    if (keyName == "Tab") return Keyboard::Tab;
    if (keyName == "Numpad 0") return Keyboard::Numpad0;
    if (keyName == "Numpad 1") return Keyboard::Numpad1;
    if (keyName == "Numpad 2") return Keyboard::Numpad2;
    if (keyName == "Numpad 3") return Keyboard::Numpad3;
    if (keyName == "Numpad 4") return Keyboard::Numpad4;
    if (keyName == "Numpad 5") return Keyboard::Numpad5;
    if (keyName == "Numpad 6") return Keyboard::Numpad6;
    if (keyName == "Numpad 7") return Keyboard::Numpad7;
    if (keyName == "Numpad 8") return Keyboard::Numpad8;
    if (keyName == "Numpad 9") return Keyboard::Numpad9;
    if (keyName == "Left Shift") return Keyboard::LShift;
    if (keyName == "Right Shift") return Keyboard::RShift;
    if (keyName == "Left Control") return Keyboard::LControl;
    if (keyName == "Right Control") return Keyboard::RControl;
    if (keyName == "Left Alt") return Keyboard::LAlt;
    if (keyName == "Right Alt") return Keyboard::RAlt;
    
    return Keyboard::Unknown; // Default case
}
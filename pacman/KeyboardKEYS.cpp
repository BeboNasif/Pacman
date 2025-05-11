#include "Menu.h"

string Menu::keyboardKeyToString(sf::Keyboard::Key key) {
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
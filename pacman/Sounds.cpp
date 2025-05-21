#include "Sounds.h"

void Sounds::music(int n) {

	if (n == 0)
		bgmusic.openFromFile("Assets/Sounds/pacman menu sound.mp3");
	bgmusic.setLoop(true);
	bgmusic.play();
}
void Sounds::startup() {
	buf1.loadFromFile("Assets/Sounds/startup.wav");
	so.setBuffer(buf1);
	so.play();
}
void Sounds::chasing(bool hunting) {
	if (hunting) {
		buf1.loadFromFile("Assets/Sounds/hunting.wav");
		so2.setBuffer(buf1);
	}
	else {
		buf1.loadFromFile("Assets/Sounds/chasing.wav");
		so2.setBuffer(buf1);
	}
	so2.setLoop(true);
	so2.play();
}
void Sounds::eating() {
	buf4.loadFromFile("Assets/Sounds/eating.wav");
	so4.setBuffer(buf4);
	so4.play();
}
void Sounds::eat_ghost() {
	buf3.loadFromFile("Assets/Sounds/eating_ghost.wav");
	so5.setBuffer(buf3);
	so5.play();
}
void Sounds::dying() {
	buf2.loadFromFile("Assets/Sounds/dying.wav");
	so6.setBuffer(buf2);
	so6.play();
}
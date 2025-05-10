#include "Sounds.h"
extern int PLayer1;
extern int PLayer2;

Sounds characters_Sound[5];

void Sounds::LoadMusic(int n)
{
	if (n == 1)
		buf1.loadFromFile("Assets//Sounds//menu_change.ogg");
}


void Sounds::music(int n) {

	if (n == 0)
		bgmusic.openFromFile("Assets//Sounds//pacman menu sound.mp3");
	/*else if (n == 1)
		bgmusic.openFromFile("Assets//Sounds//gameplay music.ogg");*/
	bgmusic.setLoop(true);
	//bgmusic.play();

}
void Sounds::change_option_Sound() {

	LoadMusic(1);
	so.setBuffer(buf1);
	//so.play();
}
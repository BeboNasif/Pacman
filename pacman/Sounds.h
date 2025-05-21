#pragma once
#include "include.h"
class Sounds
{
private:
    int k = 0;
    SoundBuffer buf1, buf2, buf3, buf4;
    
public:
    Music bgmusic;
    Sound so, so2, so4, so5, so6, so7, so8, so9, so10, so11, so12, so13;
    void music(int n);
	void startup();
	void chasing(bool hunting);
	void eating();
	void eat_ghost();
	void dying();
};
#pragma once
#include "SDL/SDL_mixer.h"

class MusicManager
{
public:
	MusicManager(void);
	~MusicManager(void);

	bool Init(void);
	void Load(char *musicfilename);
	void Play(int loops);
	void Pause();
	void FadeOut();
	bool Sonant();
private:
	Mix_Music *music;
};

#include "MusicManager.h"
#include "const.h"
#include "fileManager.h"

MusicManager::MusicManager(void) {
	music = NULL;
}

MusicManager::~MusicManager(void) {
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

bool MusicManager::Init() {
	return Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024);
}

void MusicManager::Load(char *musicfilename) 
{
	if (music != NULL) { 
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}
	Mix_VolumeMusic(MIX_MAX_VOLUME);
	//Get the bitmap's buffer and size from the resource file
	int filesize = 0;
	char *buffer = GetBufferFromResource(musicfilename, &filesize);
	
	//Load the buffer into a surface using RWops
	SDL_RWops *rw = SDL_RWFromMem(buffer, filesize);
	//SDL_Surface *temp = SDL_LoadBMP_RW(rw, 1);
	music = Mix_LoadMUS_RW(rw);
	
	//Release the bitmap buffer memory
	//free(buffer);
}

void MusicManager::Play(int loops) {
	Mix_PlayMusic(music, loops);
}

void MusicManager::Pause() {
	Mix_PauseMusic();
}

void MusicManager::FadeOut() {
	Mix_FadeOutMusic(500);
}

bool MusicManager::Sonant() {
	return (Mix_PlayingMusic() == 1) && (Mix_FadingMusic() != MIX_FADING_OUT);
}
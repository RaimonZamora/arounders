#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class IntroController
{
public:
	IntroController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);

	bool Init();
	void Go(GameInfo *gameInfo);
	void Finalize(void);

private:
	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;

	SDL_Surface *fondo;
};

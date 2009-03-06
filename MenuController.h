#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class MenuController
{
public:
	MenuController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~MenuController(void);

	bool Init();
	void Go(GameInfo *gameInfo);
	void Finalize(void);

private:
	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;

	SDL_Surface *fondo;
	SDL_Surface *cursor;
};

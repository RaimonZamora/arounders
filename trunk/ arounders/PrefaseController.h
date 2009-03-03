#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class PrefaseController
{
public:
	PrefaseController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~PrefaseController(void);

	bool Init();
	void Go(GameInfo *gameInfo);
	void Finalize(void);

private:
	char *formatejar(int numero);
	void CarregarMapa(int numMapa);

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;

	SDL_Surface *fondo;
	SDL_Surface *cursor;
	SDL_Surface *font;

	int numArounders;
	int aroundersNec;
};

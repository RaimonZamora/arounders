#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class PostfaseController
{
public:
	PostfaseController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~PostfaseController(void);

	bool Init();
	void Go(GameInfo *gameInfo);
	void Finalize(void);

private:
	char *ObtenerPasswordDeFase(int fase);
	bool MostrarVictoria(GameInfo *gameInfo);

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;

	SDL_Surface *fondo;
	SDL_Surface *font;
};

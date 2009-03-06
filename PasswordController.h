#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class PasswordController
{
public:
	PasswordController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~PasswordController(void);

	bool Init();
	void Go(GameInfo *gameInfo);
	void Finalize(void);

private:
	char ObtenerTecla();
	int ObtenerFaseDePassword(char *password);

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;

	SDL_Surface *fondo;
	SDL_Surface *cursor;
	SDL_Surface *font;

};

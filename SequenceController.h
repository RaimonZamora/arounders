#pragma once
#include "SDL/SDL.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "GameInfo.h"

class SequenceController
{
public:
	SequenceController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~SequenceController(void);

	bool Init();
	void Go(GameInfo *pGameInfo);
	void Finalize(void);

private:
	void ProcesarSecuencia(char *file);

	bool Esperar(Uint32 temps);
	void FadeIn(char *archivo);
	void Show(char *archivo);
	void Print(int x, int y, Uint32 color, char *texto);
	void Musica(char *archivo, int loop);
	void FadeOut();
	void FadeOutWithMusic();

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;
	GameInfo *gameInfo;

	SDL_Surface *fondo;
	SDL_Surface *font;
};

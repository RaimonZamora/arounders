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
	void SequenciaInicial();
	void Sequencia00();
	void Sequencia05();
	void Sequencia10();
	void Sequencia15();
	void Sequencia20();
	void Sequencia25();
	void Sequencia30();

	bool Esperar(Uint32 temps);
	void FadeIn(char *archivo);
	void Show(char *archivo);
	void Print(int x, int y, Uint32 color, char *texto);
	void FadeOut();

	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;
	GameInfo *gameInfo;

	SDL_Surface *fondo;
	SDL_Surface *font;
};

#pragma once
#include <list>
#include "SDL/SDL.h"
#include "GameInfo.h"
#include "DrawManager.h"
#include "InputManager.h"
#include "MusicManager.h"
#include "AiguaProcesor.h"
#include "MarcadorProcesor.h"
#include "ArounderProcesor.h"

class GameController
{
public:
	GameController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager);
	~GameController(void);

	bool Init();
	void Go(GameInfo *pGameInfo);
	void Finalize(void);

private:
	DrawManager *drawManager;
	InputManager *inputManager;
	MusicManager *musicManager;
	AiguaProcesor *aiguaProcesor;
	MarcadorProcesor *marcadorProcesor;

	ArounderProcesor *primerArounder;
	ArounderProcesor *arounderSeleccionat;

	SDL_Surface *mapa;
	SDL_Surface *fondo;
	SDL_Surface *sprites;
	SDL_Surface *puerta;
	SDL_Surface *font1;
	SDL_Surface *cursor;
	SDL_Surface *marca;
	SDL_Surface *menu;

	GameInfo *gameInfo;

	int startTicks;
	int currentTicks;

	int xInicial;
	int yInicial;
	int xFinal;
	int yFinal;

	void PintarEscena();
	void PintarCursor();
	void Pausa();
	bool Menu();
	void CarregarMapa(int numMapa);
	char *formatejar(int numero);

};

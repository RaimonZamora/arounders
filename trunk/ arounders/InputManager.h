#pragma once
#include "SDL/SDL.h"
#include "GameInfo.h"

class InputManager 
{
public:
	InputManager(GameInfo *pGameInfo);
	~InputManager(void);
	
	void Update();
	bool Pausa();
	bool EixirPausa();
	bool Menu();
	bool BotoPulsat(int boto);
	bool BotoSoltat(int boto);

	bool TeclaPulsada(int tecla);

	bool cualquierTecla;
	bool teclaSoltada;
	bool eixir;
	int mouseX;
	int mouseY;
	bool mouseButtonLeft;
	bool mouseButtonRight;
	bool mouseButtonMiddle;

private:
	Uint8 *keystates;
	GameInfo *gameInfo;

	SDL_Event event;
	bool oldMouseButtonLeft;
	bool oldMouseButtonRight;
	bool oldMouseButtonMiddle;

};
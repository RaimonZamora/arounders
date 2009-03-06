#pragma once
#include "DrawManager.h"

class AiguaProcesor
{
public:
	AiguaProcesor(DrawManager *p_drawManager, int p_fase);
	~AiguaProcesor(void);

	void Pintar();
	void Procesar();

private:
	DrawManager *drawManager;
	
	SDL_Surface *grafic;
	int frame1;
	int frame2;
	int fase;
};

#pragma once
#include "DrawManager.h"

class MarcadorProcesor
{
public:
	MarcadorProcesor(DrawManager *pDrawManager);
	~MarcadorProcesor(void);

	void Pintar(SDL_Surface *font, int accio, int prevista);
	int Procesar(int mouseX, int mouseY);

	int numArounders;
	int AroundersNec;
	int numAroundersArrivats;
	int numAroundersEixits;
	int numAroundersMorts;
	int orientacioInicial;

	int numParar;
	int numCavar;
	int numEscalar;
	int numPerforar;
	int numEscalera;
	int numPasarela;
	int numCorda;

	int velocitat;

private:
	char *formatejar(int numero);

	DrawManager *drawManager;
	SDL_Surface *boto;

	int contador;
	
};

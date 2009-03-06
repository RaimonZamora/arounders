#pragma once
#include "DrawManager.h"
#include "MarcadorProcesor.h"
#include "const.h"

const int ORIENT_DRETA    = 0;
const int ORIENT_ESQUERRA = 8;

class ArounderProcesor
{
public:
	ArounderProcesor(DrawManager *pDrawManager, MarcadorProcesor *pMarcadorProcesor, SDL_Surface *pSprites, SDL_Surface *pMapa, int pxInicial, int pyInicial, int pxFinal, int pyFinal );
	~ArounderProcesor(void);
	ArounderProcesor *Afegir();

	void Pintar();
	void Procesar();
	ArounderProcesor *Seleccionar(int mouseX, int mouseY);
	void AbortarAccio();

	ArounderProcesor *anterior;
	ArounderProcesor *siguiente;

	int prevista;
	int accio;
	int X;
	int Y;
	int O;

	Uint32 colorEscalo;
	Uint32 colorCorda;

private:
	SDL_Rect clip;
	SDL_Surface *sprites;
	SDL_Surface *mapa;
	SDL_Surface *explosio;
	Uint32 *pixels;

	int frame;
	int frameX;
	int frameY;
	int altura;

	int xInicial;
	int yInicial;
	int xFinal;
	int yFinal;

	DrawManager *drawManager;
	MarcadorProcesor *marcadorProcesor;

	void initMort();
	void initArrivat();
	void initCaminar();
	void initCaure();
	void initParar();
	void initCavar();
	void initEscalar();
	void initPerforar();
	void initEscalera();
	void initPasarela();
	void initCorda();
	void initPujarCorda();
	void initBaixarCorda();
	void initSuicidi();

	void Matar();

	void procesarCaminar();
	void procesarCaure();
	void procesarParar();
	void procesarCavar();
	void procesarEscalar();
	void procesarPerforar();
	void procesarEscalera();
	void procesarPasarela();
	void procesarCorda();
	void procesarPujarCorda();
	void procesarBaixarCorda();
	void procesarSuicidi();

	bool blockCaure(int desfase = 0);
	bool blockCaminar();
	bool blockArounder();
	bool blockParet(int desfase = 0);
	bool blockTecho(int desfase = 0);
	bool blockPrecipici();
	bool blockPrecipiciPasarela();
	bool blockNovaCorda();
	bool blockCordaBaixar(int desfase = 0);
	bool blockCordaPujar(int desfase = 0);
	bool pujarEscalo();
	bool baixarEscalo();

	bool checkArounderAnt(int x, int y, int o);
	bool checkArounderSig(int x, int y, int o);

	void doCavar();
	void doPerforar();
	void doEscalera(int desfase = 0);
	void doCorda();

	Uint32 get_pixel( int x, int y );
	void put_pixel( int x, int y, Uint32 pixel );
};

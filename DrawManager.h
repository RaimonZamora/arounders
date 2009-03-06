#pragma once
#include "SDL/SDL.h"

class DrawManager
{
public:
	DrawManager(int pMode);
	~DrawManager(void);

	bool Init(void);
	bool Flip(void);

	SDL_Surface *LoadBitmap(char *bitmapfilename, bool doColorKey = false);
	SDL_Surface *LoadMask(char *bitmapfilename);
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	void Blit( int x, int y, SDL_Surface* source, SDL_Rect* clip = NULL );
	void Print( int x, int y, SDL_Surface* source, int w, int h, char *text, SDL_Surface* dest = NULL );
	void FadeOut();
	void FadeIn();
	void FadeOutGray();
	void FadeInGray();
	void DrawFadedBack();
	SDL_Surface *LoadFont(char *bitmapfilename, Uint32 color = 0xFFFFFFFF);

	int mode;

	Uint32 color_blanco;
	Uint32 color_rojo;
	Uint32 color_verde;
	Uint32 color_azul;
private:
	SDL_Surface *screen;
	SDL_Surface *screenBig;
	SDL_Surface *black;
	SDL_Surface *temp;
	SDL_Surface *faded;
	int alpha;

	Uint32 get_pixel32( SDL_Surface *surface, int x, int y );
	void put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );

};

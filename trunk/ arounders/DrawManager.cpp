#include "DrawManager.h"
#include "const.h"
#include "fileManager.h"
#include "SDL/SDL_image.h"



DrawManager::DrawManager(int pMode)
{
	mode = pMode;
	screen = NULL;
	screenBig = NULL;
}

DrawManager::~DrawManager(void)
{
	SDL_FreeSurface(temp);
	SDL_FreeSurface(black);
	SDL_FreeSurface(faded);
}

bool DrawManager::Init(void)
{
	switch (mode) {
		case 1:
			screenBig = SDL_SetVideoMode( SCREEN_WIDTH<<1, SCREEN_HEIGHT<<1, SCREEN_BPP, SDL_SWSURFACE );
			if( screenBig == NULL ) { return false; }
			screen = LoadBitmap("black.gif");
			break;
		case 2:
			screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_FULLSCREEN );
			if( screen == NULL ) { return false; }
			break;
		default:
			screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
			if( screen == NULL ) { return false; }
			break;
	}
	color_rojo = SDL_MapRGB( screen->format, 255, 0, 0 );
	color_verde = SDL_MapRGB( screen->format, 0, 255, 0 );
	color_azul = SDL_MapRGB( screen->format, 0, 0, 255 );

	black = LoadBitmap("black.gif");
	temp = LoadBitmap("black.gif");
	faded = LoadBitmap("black.gif");

	return true;
}

bool DrawManager::Flip(void)
{
	if (mode == 1) {
		for (int x=0; x<320; x++) {
			for (int y=0; y<200; y++) {
				put_pixel32(screenBig,  x<<1   ,  y<<1   , get_pixel32(screen, x, y));
				put_pixel32(screenBig, (x<<1)+1,  y<<1   , get_pixel32(screen, x, y));
				put_pixel32(screenBig,  x<<1   , (y<<1)+1, get_pixel32(screen, x, y));
				put_pixel32(screenBig, (x<<1)+1, (y<<1)+1, get_pixel32(screen, x, y));
			}
		}
		if (SDL_Flip( screenBig ) == -1 ) {
			return true;
		} else {
			return false;
		}
	} else {
		if (SDL_Flip( screen ) == -1 ) {
			return true;
		} else {
			return false;
		}
	}
}

SDL_Surface *DrawManager::LoadBitmap(char *bitmapfilename, bool doColorKey) 
{
	//Get the bitmap's buffer and size from the resource file
	int filesize = 0;
	char *buffer = GetBufferFromResource(bitmapfilename, &filesize);
	
	//Load the buffer into a surface using RWops
	SDL_RWops *rw = SDL_RWFromMem(buffer, filesize);
	//SDL_Surface *temp = SDL_LoadBMP_RW(rw, 1);
	SDL_Surface *temp = IMG_Load_RW(rw, 1);
	
	//Release the bitmap buffer memory
	free(buffer);
	
	//Were we able to load the bitmap?
	if (temp == NULL) 
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Convert the image to optimal display format
	SDL_Surface *image;
	image = SDL_DisplayFormat(temp);
	
	if (doColorKey) {
		Uint32 colorkey = SDL_MapRGB( image->format, 0, 0, 0 );
		SDL_SetColorKey( image, SDL_SRCCOLORKEY, colorkey );
	}

	//Free the temporary surface
	SDL_FreeSurface(temp);
	
	//Return our loaded image
	return image;
}


SDL_Surface *DrawManager::LoadMask(char *bitmapfilename)
{
	//Get the bitmap's buffer and size from the resource file
	int filesize = 0;
	char *buffer = GetBufferFromResource(bitmapfilename, &filesize);
	
	//Load the buffer into a surface using RWops
	SDL_RWops *rw = SDL_RWFromMem(buffer, filesize);
	//SDL_Surface *temp = SDL_LoadBMP_RW(rw, 1);
	SDL_Surface *temp = IMG_Load_RW(rw, 1);
	
	//Release the bitmap buffer memory
	free(buffer);
	
	//Were we able to load the bitmap?
	if (temp == NULL) 
	{
		printf("Unable to load bitmap: %s\n", SDL_GetError());
		exit(1);
	}
	
	//Convert the image to optimal display format
	SDL_Surface *image;
	image = SDL_DisplayFormat(temp);
	
	Uint32 colorkey = SDL_MapRGB( image->format, 255, 255, 255 );
	SDL_SetColorKey( image, SDL_SRCCOLORKEY, colorkey );

	//Free the temporary surface
	SDL_FreeSurface(temp);
	
	//Return our loaded image
	return image;
}


void DrawManager::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    SDL_Rect offset;
    
    offset.x = x;
    offset.y = y;

	SDL_BlitSurface( source, clip, destination, &offset );
}

void DrawManager::Blit( int x, int y, SDL_Surface* source, SDL_Rect* clip )
{
		apply_surface(x, y, source, screen, clip);
}

void DrawManager::Print( int x, int y, SDL_Surface* source, int w, int h, char *text, SDL_Surface *dest ) {
	int index = 0;
	SDL_Rect clip;

	clip.y = 0;
	clip.w = w;
	clip.h = h;

	if (dest == NULL) dest = screen;

	while (text[index] > 0) {
		clip.x = (text[index] - 32) * 7;
		apply_surface(x+(index*w), y, source, dest, &clip);
		index++;
	}
}

void DrawManager::FadeOut()
{
	alpha = 0;
	apply_surface(0, 0, screen, temp);

	int currentTicks;
	int startTicks = SDL_GetTicks();

	while (alpha < 255) {
		SDL_SetAlpha( black, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
		Blit(0,0, temp);
		Blit(0,0, black);
		Flip();

		currentTicks = SDL_GetTicks() - startTicks;
		if( currentTicks >= 10 ) {
			startTicks = SDL_GetTicks();
			alpha+=8;
		} 

	}
}

void DrawManager::FadeIn()
{
	alpha = 255;
	apply_surface(0, 0, screen, temp);

	int currentTicks;
	int startTicks = SDL_GetTicks();

	while (alpha > 0) {
		SDL_SetAlpha( black, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
		Blit(0,0, temp);
		Blit(0,0, black);
		Flip();

		currentTicks = SDL_GetTicks() - startTicks;
		if( currentTicks >= 10 ) {
			startTicks = SDL_GetTicks();
			alpha-=8;
		} 
	}
}

void DrawManager::FadeOutGray()
{
	alpha = 0;
	apply_surface(0, 0, screen, temp);

	int currentTicks;
	int startTicks = SDL_GetTicks();
	
	while (alpha < 127) {
		SDL_SetAlpha( black, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
		Blit(0,0, temp);
		Blit(0,0, black);
		Flip();
		
		currentTicks = SDL_GetTicks() - startTicks;
		if( currentTicks >= 10 ) {
			startTicks = SDL_GetTicks();
			alpha+=8;
		} 
	}
	apply_surface(0, 0, screen, faded);
}

void DrawManager::FadeInGray()
{
	alpha = 127;
	//apply_surface(0, 0, screen, temp);
	
	int currentTicks;
	int startTicks = SDL_GetTicks();
	
	while (alpha > 0) {
		SDL_SetAlpha( black, SDL_SRCALPHA | SDL_RLEACCEL, alpha );
		Blit(0,0, temp);
		Blit(0,0, black);
		Flip();
		
		currentTicks = SDL_GetTicks() - startTicks;
		if( currentTicks >= 10 ) {
			startTicks = SDL_GetTicks();
			alpha-=8;
		} 
	}
}

void DrawManager::DrawFadedBack() {
	Blit(0,0, faded);
}

SDL_Surface *DrawManager::LoadFont(char *bitmapfilename, Uint32 color)
{
	if (color == 0xFFFFFFFF) {
		return LoadBitmap(bitmapfilename, true);
	} else {
		SDL_Surface *temporal = LoadBitmap(bitmapfilename, true);
		if( SDL_MUSTLOCK( temporal ) ) { SDL_LockSurface( temporal ); }

		for (int x=0; x<temporal->w; x++) {
			for (int y=0; y<temporal->h; y++) {
				if (get_pixel32(temporal, x, y) == 0xFFFFFF) {
					put_pixel32(temporal, x, y, color);
				}
			}
		}

		if( SDL_MUSTLOCK( temporal ) ) { SDL_UnlockSurface( temporal ); }

		return temporal;
	}
}

Uint32 DrawManager::get_pixel32( SDL_Surface *surface, int x, int y )
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[ ( y * surface->w ) + x ];
}

void DrawManager::put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[ ( y * surface->w ) + x ] = pixel;
}


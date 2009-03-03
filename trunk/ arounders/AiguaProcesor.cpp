#include "AiguaProcesor.h"

AiguaProcesor::AiguaProcesor(DrawManager *p_drawManager, int p_fase)
{
	drawManager = p_drawManager;
	fase = p_fase;

	grafic = drawManager->LoadFont("aigua.gif");
	frame1 = 0;
	frame2 = 5;
}

AiguaProcesor::~AiguaProcesor(void)
{
	SDL_FreeSurface(grafic);
}

void AiguaProcesor::Pintar() {
	int frames1[10] = {0,1,2,1,0,3,4,5,4,3};
	int frames2[10] = {6,7,8,7,6,9,10,11,10,9};
	int *frames;
	if ((fase+1) % 5 == 0) {
		frames = frames2;
	} else {
		frames = frames1;
	}

	SDL_Rect clip;
	clip.y = 0;
	clip.w = 16;
	clip.h = 15;

	for (int i=0;i<10;i++) {
		clip.x = frames[frame1]*16;
		drawManager->Blit(i*32, 150, grafic, &clip);
		clip.x = frames[frame2]*16;
		drawManager->Blit(16+i*32, 150, grafic, &clip);
	}
}

void AiguaProcesor::Procesar() {
	frame1++;
	frame2++;

	if (frame1 == 10) frame1 = 0;
	if (frame2 == 10) frame2 = 0;
}
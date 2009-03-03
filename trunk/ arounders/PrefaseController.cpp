#include "PrefaseController.h"
#include "const.h"
#include "fileManager.h"

PrefaseController::PrefaseController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

PrefaseController::~PrefaseController(void) {
}

bool PrefaseController::Init() {
	fondo = drawManager->LoadBitmap("prefase.gif");
	cursor = drawManager->LoadFont("cursor.gif");
	font = drawManager->LoadFont("fuente1.gif");

	return true;
}

void PrefaseController::Go(GameInfo *gameInfo) {
	bool salir = false;

	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	CarregarMapa(gameInfo->fase);

	drawManager->Blit(0,0, fondo);
	drawManager->Print(130, 60, font, 7, 5, "NIVELL");
	drawManager->Print(179, 60, font, 7, 5, formatejar(gameInfo->fase+1));

	drawManager->Print(80, 100, font, 7, 5, formatejar(numArounders));
	drawManager->Print(101, 100, font, 7, 5, "AROUNDERS DISPONIBLES");

	drawManager->Print(80, 110, font, 7, 5, formatejar(aroundersNec));
	drawManager->Print(101, 110, font, 7, 5, "AROUNDERS NECESSARIS");
	drawManager->FadeIn();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, fondo);

		drawManager->Print(130, 60, font, 7, 5, "NIVELL");
		drawManager->Print(179, 60, font, 7, 5, formatejar(gameInfo->fase+1));

		drawManager->Print(80, 100, font, 7, 5, formatejar(numArounders));
		drawManager->Print(101, 100, font, 7, 5, "AROUNDERS DISPONIBLES");

		drawManager->Print(80, 110, font, 7, 5, formatejar(aroundersNec));
		drawManager->Print(101, 110, font, 7, 5, "AROUNDERS NECESSARIS");

		drawManager->Blit(inputManager->mouseX, inputManager->mouseY, cursor);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			gameInfo->estado = ESTADO_JUEGO;
			salir = true;
		}
	}
	musicManager->FadeOut();
	drawManager->FadeOut();
}

void PrefaseController::Finalize(void) {
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(cursor);
	SDL_FreeSurface(font);
}

void PrefaseController::CarregarMapa(int numMapa)
{
	int filesize = 0;
	char *buffer = GetBufferFromResource("MAPES.BAL", &filesize);

	int punter = numMapa * 212;

	int tileSet = buffer[punter++];
	punter += 2;
	numArounders = buffer[punter++];
	aroundersNec = buffer[punter++];
}

char *PrefaseController::formatejar(int numero) {
	char *resultat;
	resultat = (char *) malloc(3);

	if (numero > 9) {
			resultat[0] = (numero / 10) + 48;
			resultat[1] = (numero % 10) + 48;
	} else {
			resultat[0] = 48;
			resultat[1] = (numero % 10) + 48;
	}

	resultat[2] = '\0';

	return resultat;
}

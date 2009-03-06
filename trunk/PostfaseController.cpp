#include "PostfaseController.h"
#include "const.h"
#include "fileManager.h"

PostfaseController::PostfaseController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

PostfaseController::~PostfaseController(void) {
}

bool PostfaseController::Init() {
	fondo = drawManager->LoadBitmap("postfase.gif");
	font = drawManager->LoadFont("fuente2.gif", drawManager->color_rojo);

	return true;
}

void PostfaseController::Go(GameInfo *gameInfo) {
	bool salir = false;

	if (gameInfo->fase == 30) {
		gameInfo->estado = ESTADO_SEQUENCIA;
		return;
	} else {
		musicManager->Load( "mus3.mp3");
		musicManager->Play(-1);
	}

	if (gameInfo->fase % 5 == 0 && gameInfo->fase != 30) {
		salir = MostrarVictoria(gameInfo);
	}

	if (salir) {
		gameInfo->estado = ESTADO_SALIR;
		return;
	}

	char *password = ObtenerPasswordDeFase(gameInfo->fase);
	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	drawManager->Blit(0,0, fondo);
	drawManager->Print(175, 166, font, 7, 6, password);
	drawManager->FadeIn();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, fondo);
		drawManager->Print(175, 166, font, 7, 6, password);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			gameInfo->estado = ESTADO_SEQUENCIA;
			salir = true;
		}
	}
	drawManager->FadeOut();
}

char *PostfaseController::ObtenerPasswordDeFase(int fase) {
	int filesize = 0;
	char *buffer = GetBufferFromResource("offsets.bal", &filesize);

	int punter = (fase-1)*11;

	char *passFile = (char *) malloc(11);

	for (int i=0;i<10;i++) {
		punter++;
		passFile[i] = ((Uint8)buffer[punter]) - (101+i);
	}

	return passFile;
}

bool PostfaseController::MostrarVictoria(GameInfo *gameInfo) {
	bool salir = false;

	switch (gameInfo->fase) {
		case 5:
			fondo = drawManager->LoadBitmap("final01.GIF");
			break;
		case 10:
			fondo = drawManager->LoadBitmap("final02.GIF");
			break;
		case 15:
			fondo = drawManager->LoadBitmap("final03.GIF");
			break;
		case 20:
			fondo = drawManager->LoadBitmap("final04.GIF");
			break;
		case 25:
			fondo = drawManager->LoadBitmap("final05.GIF");
			break;
	}

	drawManager->Blit(0,0, fondo);
	drawManager->FadeIn();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, fondo);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			gameInfo->estado = ESTADO_SEQUENCIA;
			salir = true;
		}
	}
	drawManager->FadeOut();

	if (gameInfo->estado == ESTADO_SALIR) {
		return true;
	} else {
		return false;
	}
}

void PostfaseController::Finalize(void) {
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(font);
}


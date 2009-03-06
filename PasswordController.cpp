#include "PasswordController.h"
#include "const.h"
#include "fileManager.h"

PasswordController::PasswordController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

PasswordController::~PasswordController(void) {
}

bool PasswordController::Init() {
	fondo = drawManager->LoadBitmap("prefase.gif");
	cursor = drawManager->LoadFont("cursor.gif");
	font = drawManager->LoadFont("fuente1.gif");

	return true;
}

void PasswordController::Go(GameInfo *gameInfo) {
	bool salir = false;
	char tecla = -1;
	char password[11] = "          ";
	char indice = 0;

	if ( !Init() ) { salir = true; gameInfo->estado = ESTADO_SALIR; }

	drawManager->Blit(0,0, fondo);
	drawManager->Print(95, 80, font, 7, 5, "ESCRIU EL PASSWORD");
	drawManager->FadeIn();

	while ( !salir ) {
		inputManager->Update();

		drawManager->Blit(0,0, fondo);
		drawManager->Print(95, 80, font, 7, 5, "ESCRIU EL PASSWORD");
		drawManager->Print(123, 140, font, 7, 5, password);
		drawManager->Flip();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
		}

		if (inputManager->cualquierTecla && tecla == -1) {
			if (inputManager->TeclaPulsada(SDLK_BACKSPACE) && indice > 0) {
				indice--;
				password[indice] = 32;
				tecla = SDLK_BACKSPACE;
			} else {
				tecla = ObtenerTecla();
				if (tecla != -1) {
					password[indice] = tecla;
					indice++;
					if (indice == 10) {
						gameInfo->fase = ObtenerFaseDePassword(password);
						gameInfo->estado = ESTADO_SEQUENCIA;
						salir = true;
					}
				}
			}
		}
		if (inputManager->teclaSoltada) {
			tecla = -1;
		}
	}
	drawManager->FadeOut();
}

char PasswordController::ObtenerTecla() {
	char tecla = -1;
	for (int i=48; i<=57; i++) {
		if (inputManager->TeclaPulsada(i)) tecla = i;
	}
	for (int i=97; i<=122; i++) {
		if (inputManager->TeclaPulsada(i)) tecla = i-32;
	}
	return tecla;
}

int PasswordController::ObtenerFaseDePassword(char *password) {
	int filesize = 0;
	char *buffer = GetBufferFromResource("offsets.bal", &filesize);

	int punter = 0;

	bool salir = false;
	char passFile[11] = "          ";
	int numPassword = 0;

	while ( numPassword < 30 && !salir ) {
		for (int i=0;i<10;i++) {
			punter++;
			passFile[i] = buffer[punter] - (101+i);
		}
		punter++;

		salir = true;
		for (int i=0;i<10;i++) {
			if (passFile[i] != password[i]) salir = false;
		}
		numPassword++;
	}

	if (!salir) numPassword = 0;

	return numPassword;
}

void PasswordController::Finalize(void) {
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(cursor);
	SDL_FreeSurface(font);
}

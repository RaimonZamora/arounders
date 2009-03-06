#include "SequenceController.h"
#include "const.h"
#include "fileManager.h"

SequenceController::SequenceController(DrawManager *p_drawManager, InputManager *p_inputManager, MusicManager *p_musicManager) {
	drawManager = p_drawManager;
	inputManager = p_inputManager;
	musicManager = p_musicManager;
}

SequenceController::~SequenceController(void) {
}

bool SequenceController::Init() {
	return true;
}

void SequenceController::Go(GameInfo *pGameInfo) {
	bool salir = false;
	int pantalla = 0;
	char *file;
	int filesize;

	gameInfo = pGameInfo;

	if (gameInfo->fase % 5 == 0 || gameInfo->fase < 0) {
		switch (gameInfo->fase) {
			case -1:
				file = GetBufferFromResource("seqIN.seq", &filesize);
				break;
			case 0:
				file = GetBufferFromResource("seq00.seq", &filesize);
				break;
			case 5:
				file = GetBufferFromResource("seq05.seq", &filesize);
				break;
			case 10:
				file = GetBufferFromResource("seq10.seq", &filesize);
				break;
			case 15:
				file = GetBufferFromResource("seq15.seq", &filesize);
				break;
			case 20:
				file = GetBufferFromResource("seq20.seq", &filesize);
				break;
			case 25:
				file = GetBufferFromResource("seq25.seq", &filesize);
				break;
			case 30:
				file = GetBufferFromResource("seq30.seq", &filesize);
				break;
		}
		ProcesarSecuencia(file);
	}
	//drawManager->FadeOut();

	if (gameInfo->estado != ESTADO_SALIR) {
		gameInfo->estado = ESTADO_PREFASE;
		if (gameInfo->fase == -1) gameInfo->estado = ESTADO_MENU;
		if (gameInfo->fase == 30) gameInfo->estado = ESTADO_MENU;
	}
}

void SequenceController::ProcesarSecuencia(char *file) {
	char numDiapositives = (char)file[0];
	int punter = 1;
	char *filename;
	char *texto;
	char tamanyCadena;
	Uint16 x;
	Uint16 y;
	Uint16 temps;
	char colorTemp;
	Uint32 color;
	bool salir = false;

	for (int j=0; j<numDiapositives; j++) {
		if (salir) {
			FadeOutWithMusic();
			break;
		}

		char tipusDiapositiva = (char)file[punter++];
		x = 0;
		y = 0;
		temps = 0;
		switch (tipusDiapositiva) {
			case DIAPO_ESPERAR:
				temps = (Uint16)(((unsigned char)file[punter+1] << 8) + (unsigned char)file[punter]);
				salir = !Esperar(temps);
				punter += 2;
				break;
			case DIAPO_FADEIN:
				tamanyCadena = file[punter++];
				filename = (char *) malloc(tamanyCadena + 1);
				for (int i=0; i<tamanyCadena; i++) filename[i] = file[punter++];
				filename[tamanyCadena] = '\0';
				FadeIn(filename);
				break;
			case DIAPO_SHOW:
				tamanyCadena = file[punter++];
				filename = (char *) malloc(tamanyCadena + 1);
				for (int i=0; i<tamanyCadena; i++) filename[i] = file[punter++];
				filename[tamanyCadena] = '\0';
				Show(filename);
				break;
			case DIAPO_PRINT:
				x = (Uint16)(((unsigned char)file[punter+1] << 8) + (unsigned char)file[punter]);
				punter+=2;
				y = (Uint16)(((unsigned char)file[punter+1] << 8) + (unsigned char)file[punter]);
				punter+=2;
				colorTemp = file[punter++];
				switch (colorTemp) {
					case 0:
						color = drawManager->color_blanco;
						break;
					case 1:
						color = drawManager->color_rojo;
						break;
					case 2:
						color = drawManager->color_verde;
						break;
					case 3:
						color = drawManager->color_azul;
						break;
				}
				tamanyCadena = file[punter++];
				texto = (char *) malloc(tamanyCadena + 1);
				for (int i=0; i<tamanyCadena; i++) texto[i] = file[punter++];
				texto[tamanyCadena] = '\0';
				Print(x, y, color, texto);
				break;
			case DIAPO_MUSICA:
				tamanyCadena = file[punter++];
				filename = (char *) malloc(tamanyCadena + 1);
				for (int i=0; i<tamanyCadena; i++) filename[i] = file[punter++];
				filename[tamanyCadena] = '\0';
				Musica(filename, -1);
				break;
			case DIAPO_FADEOUT:
				FadeOut();
				break;
			case DIAPO_FADEMUSIC:
				FadeOutWithMusic();
				break;
		}
	}

}

bool SequenceController::Esperar(Uint32 temps) {
	bool seguirIntro = true;
	bool salir = false;
	Uint32 startTicks = SDL_GetTicks();

	while ( !salir ) {
		inputManager->Update();

		if (inputManager->eixir) {
			gameInfo->estado = ESTADO_SALIR;
			salir = true;
			seguirIntro = false;
		}

		if (inputManager->cualquierTecla || inputManager->BotoPulsat(SDL_BUTTON_LEFT)) {
			salir = true;
			if (inputManager->TeclaPulsada(SDLK_ESCAPE)) seguirIntro = false;
		}

		if (SDL_GetTicks()-startTicks > temps) {
			salir = true;
		}
	}
	return seguirIntro;
}

void SequenceController::FadeIn(char *archivo) {
	fondo = drawManager->LoadBitmap( archivo);
	drawManager->Blit(0,0, fondo);
	drawManager->FadeIn();
	SDL_FreeSurface(fondo);
}

void SequenceController::Show(char *archivo) {
	fondo = drawManager->LoadBitmap( archivo);
	drawManager->Blit(0,0, fondo);
	drawManager->Flip();
	SDL_FreeSurface(fondo);
}

void SequenceController::Print(int x, int y, Uint32 color, char *texto) {
	font = drawManager->LoadFont( "fuente2.gif", color);
	drawManager->Print(x, y, font, 7, 6, texto);
	drawManager->Flip();
	SDL_FreeSurface(font);
}

void SequenceController::FadeOut() {
	drawManager->FadeOut();
}

void SequenceController::FadeOutWithMusic() {
	drawManager->FadeOut();
	musicManager->FadeOut();
}

void SequenceController::Musica(char *archivo, int loop) {
	musicManager->Load(archivo);
	musicManager->Play(loop);
}

void SequenceController::Finalize(void) {

}

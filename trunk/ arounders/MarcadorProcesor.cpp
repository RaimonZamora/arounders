#include "MarcadorProcesor.h"
#include "const.h"

MarcadorProcesor::MarcadorProcesor(DrawManager *pDrawManager)
{
	drawManager = pDrawManager;

	numAroundersArrivats = 0;
	numAroundersEixits = 0;
	numAroundersMorts = 0;

	boto = drawManager->LoadBitmap("boto.gif", true);

	velocitat = 70;
	contador = SDL_GetTicks();
}

MarcadorProcesor::~MarcadorProcesor(void)
{
	SDL_FreeSurface(boto);
}

void MarcadorProcesor::Pintar(SDL_Surface *font, int accio, int prevista) {
	drawManager->Print(23,188, font, 7, 5, formatejar(numParar));
	drawManager->Print(39,188, font, 7, 5, formatejar(numCavar));
	drawManager->Print(55,188, font, 7, 5, formatejar(numEscalar));
	drawManager->Print(71,188, font, 7, 5, formatejar(numPerforar));
	drawManager->Print(87,188, font, 7, 5, formatejar(numEscalera));
	drawManager->Print(103,188, font, 7, 5, formatejar(numPasarela));
	drawManager->Print(119,188, font, 7, 5, formatejar(numCorda));

	drawManager->Print(301,171, font, 7, 5, formatejar(numAroundersEixits-numAroundersArrivats-numAroundersMorts));
	drawManager->Print(301,177, font, 7, 5, formatejar(numArounders));
	drawManager->Print(301,183, font, 7, 5, formatejar(AroundersNec));
	drawManager->Print(301,189, font, 7, 5, formatejar(numAroundersArrivats));

	if (accio >= 10) accio = 0;
	if (prevista >= 10) prevista = 0;
	drawManager->Blit((accio*16)+5, 171, boto);

	if (prevista != accio && prevista != 0) {
		if ((SDL_GetTicks()-contador) <= 200) {
			drawManager->Blit((prevista*16)+5, 171, boto);
		} else {
			if ((SDL_GetTicks()-contador) >= 400) contador = SDL_GetTicks();
		}
	}

	if (velocitat < 70) {
		drawManager->Blit((ACCIO_SUICIDI2*16)+5, 171, boto);
	}
}

int MarcadorProcesor::Procesar(int mouseX, int mouseY) {
	int accio = -1;

	if (mouseY >= 171 && mouseY <= 187 && mouseX >= 5 && mouseX <= 165) {
		accio = (mouseX-5) / 16;

		switch(accio) {
			case ACCIO_PARAR:
				if (numParar == 0) accio = -1;
				break;
			case ACCIO_CAVAR:
				if (numCavar == 0) accio = -1;
				break;
			case ACCIO_ESCALAR:
				if (numEscalar == 0) accio = -1;
				break;
			case ACCIO_PERFORAR:
				if (numPerforar == 0) accio = -1;
				break;
			case ACCIO_ESCALERA:
				if (numEscalera == 0) accio = -1;
				break;
			case ACCIO_PASARELA:
				if (numPasarela == 0) accio = -1;
				break;
			case ACCIO_CORDA:
				if (numCorda == 0) accio = -1;
				break;
			case ACCIO_SUICIDI2:
				if (velocitat == 70) {
					velocitat = 10;
				} else {
					velocitat = 70;
				}
				accio = -1;
				break;
		}
	}
	return accio;
}

char *MarcadorProcesor::formatejar(int numero) {
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

#pragma once
#ifndef _CONST_H_

#define _CONST_H_

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 200;
const int SCREEN_BPP = 32;
const char APPLICATION_NAME[12] = "Arounders";

const int ESTADO_SALIR		= 0;
const int ESTADO_INTRO		= 1;
const int ESTADO_SEQUENCIA	= 2;
const int ESTADO_MENU		= 3;
const int ESTADO_PASSWORD	= 4;
const int ESTADO_PREFASE	= 5;
const int ESTADO_JUEGO		= 6;
const int ESTADO_POSTFASE	= 7;
const int ESTADO_MORT		= 8;
const int ESTADO_CREDITS	= 9;

const int ACCIO_CAMINAR     = 0;
const int ACCIO_PARAR		= 1;
const int ACCIO_CAVAR		= 2;
const int ACCIO_ESCALAR		= 3;
const int ACCIO_PERFORAR	= 4;
const int ACCIO_ESCALERA	= 5;
const int ACCIO_PASARELA	= 6;
const int ACCIO_CORDA		= 7;
const int ACCIO_SUICIDI		= 8;
const int ACCIO_SUICIDI2	= 9;
const int ACCIO_CAURE       = 10;
const int ACCIO_PUJARCORDA  = 11;
const int ACCIO_BAIXARCORDA = 12;
const int ACCIO_MORT	    = 13;

const int DIAPO_ESPERAR		= 0;
const int DIAPO_FADEIN		= 1;
const int DIAPO_SHOW		= 2;
const int DIAPO_PRINT		= 3;
const int DIAPO_MUSICA		= 4;
const int DIAPO_FADEOUT		= 5;
const int DIAPO_FADEMUSIC	= 6;

const int MODE_NORMAL		= 0;
const int MODE_ZOOMX2		= 1;
const int MODE_FULLSCREEN	= 2;

#endif
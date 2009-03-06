#include "GameInfo.h"
#include "const.h"

GameInfo::GameInfo(int pModeGrafic) {
	fase = -1;
	estado = ESTADO_SEQUENCIA;
	modeGrafic = pModeGrafic;
}

GameInfo::~GameInfo(void) {
}

#pragma once

class GameInfo
{
public:
	GameInfo(int pModeGrafic);
	~GameInfo(void);

	int fase;
	int puntuacion;
	int vidas;
	int estado;
	int modeGrafic;
};

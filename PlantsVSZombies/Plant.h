#pragma once


struct Plant
{
	//�����lastOneֻ��Ϊ��head���÷���
	int index;
	short PositionX;
	short PositionY;
	struct Plant *lastOne;
	struct Plant *nextOne;
	bool shouldBePlanted;
};

int GrassPositionX[9] = {290,365,445,535,610,695,775,850,945};
int GrassPositionY[4] = {230,330,430,490};
#include "Game.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	//Game Launch
	if (GameInitialize() == -1)
	{
		err("GameInitialize failed");
	}

	//printf("%s\n", getTitle());
	//������
	while (1)
	{

		onClick();//�������

		GameUpdate();
	}
	

	system("pause");

	GameStop();

	return 0;
}

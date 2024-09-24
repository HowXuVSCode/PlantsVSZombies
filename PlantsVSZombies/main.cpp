#include "Game.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	//Game Launch
	if (GameInitialize() == -1)
	{
		err("GameInitialize failed");
	}

	//printf("%s\n", getTitle());
	//主进程
	while (1)
	{

		onClick();//监听鼠标

		GameUpdate();
	}
	

	system("pause");

	GameStop();

	return 0;
}

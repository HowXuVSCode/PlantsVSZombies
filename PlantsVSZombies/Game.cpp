#include "Game.h"
#include "util.h"
#include "Plant.h"

char title[32];
IMAGE background;
IMAGE baseBar;
IMAGE cards[cardNum];
IMAGE plants[plantsNum][FPSnum];

bool isPaintPlantOnMouse = false;
int index = 0;
short MouseX = 0;
short MouseY = 0;

int fps = 0;
int cache_fps = 0;

bool hasPlanted[9][4];

struct Plant Head = { 0,0,0,NULL,NULL,false };

void loadGameImages() {
	//载入游戏资源
	loadimage(&background, (LPCTSTR)backgroundPath);
	loadimage(&baseBar,(LPCTSTR) baseBarPath);
	//因为命名原因这里应该多1 加载卡牌
	for (int i = 0; i < 3; i++)
	{
		char path[32];
		sprintf(path, "res/cards/card_%d.png", i+1);
		loadimage(&(cards[i]),(LPCTSTR)path);
	}
	//加载植物卡牌分帧
	for (int i = 0; i < 3; i++)
	{
		for (int n = 0; n < 13 ; n++)
		{
			char path[64];
			sprintf(path, "res/plants/%d/%d.png", i,n + 1);
			loadimage(&(plants[i][n]), (LPCTSTR)path);
		}
	}
}

int GameInitialize() {

	loadGameImages();

	//创建窗口大小为640x480像素
	HWND mainWindow = initgraph(900, 600,1); //参数1代表开启控制台窗口
	//_getch();
	if (mainWindow == NULL)
	{
		err("mainWindow create failed !");
		return -1;
	}
	//设置窗口标题
	if (!SetWindowText(mainWindow,(LPCTSTR) getTitle()))
	{
		err("mainWindow title change failed !");
	}
	return 0;
}

void GameUpdate() {

	//麻痹的一个缓冲函数就行了无敌了
	BeginBatchDraw();//开始缓冲
	//每帧更新背景图 应在initialize之后调用
	//前两个参数是坐标渲染位置
	putimage(0, 0, &background);
	putimagePNG(250,0,&baseBar);
	//绘制植物卡牌
	for (int i = 0; i < cardNum; i++)
	{
		putimagePNG(CardBoxLeftPostion + CardWidth * i, CardBoxTopPostion, &cards[i]);
	}

	//判定鼠标处更新出植物
	if (isPaintPlantOnMouse && MouseX != 0 && MouseY != 0)
	{
		putimagePNG(MouseX - 37, MouseY - 37, &(plants[index][fps]));//帧数管理
	}

	struct Plant *toBePlanted = nullptr;
	if (Head.nextOne != nullptr && Head.lastOne != nullptr)
	{
		toBePlanted = Head.nextOne;
	}

	if (toBePlanted != nullptr)
	{
		do {
			//渲染种植的植物
			if (toBePlanted->index == 2)
			{
				//大嘴花单独判断
				putimagePNG(GrassPositionX[toBePlanted->PositionX] - 35, GrassPositionY[toBePlanted->PositionY] - 60, &(plants[toBePlanted->index][fps]));
			}
			else
			{
				putimagePNG(GrassPositionX[toBePlanted->PositionX] - 30, GrassPositionY[toBePlanted->PositionY] - 40, &(plants[toBePlanted->index][fps]));
			}
			
			//判断进行下一个渲染
			if (toBePlanted->nextOne == nullptr)
			{
				break;
			}
			toBePlanted = toBePlanted->nextOne;
		} while (1);
	}

	EndBatchDraw();//结束缓冲

	//FPS更新
	FPS_Control();
	
}

void FPS_Control() {
	//FPS控制

	Sleep(10);
	cache_fps++;

	if (cache_fps == 5)
	{
		fps++;
		cache_fps = 0;
	}

	if (fps == 12)
	{
		fps = 0;
	}



}

int GameStop() {
	closegraph();
	//调试设置
	return 0;
}



void onClick() {
	
	ExMessage msg;
	if (peekmessage(&msg)) {
		//监听鼠标变化
		//监听左键拉出植物卡牌
		if (msg.message == WM_LBUTTONDOWN)
		{
			
			if (msg.x > CardBoxLeftPostion && msg.x < CardBoxRightPostion && msg.y > CardBoxTopPostion && msg.y < CardBoxButtomPostion)
				//判断位于框中
			{	
				index = (msg.x - CardBoxLeftPostion) / CardWidth;
				isPaintPlantOnMouse = true;
				
			}
		}

		if (msg.message == WM_MOUSEMOVE && isPaintPlantOnMouse)
		{
			MouseX = msg.x;
			MouseY = msg.y;
		}

		if (msg.message == WM_LBUTTONUP && isPaintPlantOnMouse)
		{
			//log("Left Up");
			//松开左键
			MouseX = 0;
			MouseY = 0;
			isPaintPlantOnMouse = false;
			//判定是否为种植位置
			if (msg.x > GrassLeftPositon && msg.y < GrassbuttomPositon && msg.y > GrassTopPositon)
			{
				//malloc一个植物结构体存储位置和index，并且使用EmptyPlant标定
				struct Plant *plant = (struct Plant *) malloc(sizeof(Plant));
				plant->lastOne = NULL;
				plant->nextOne = NULL;
				plant->index = index;
				plant->shouldBePlanted = true;
				getBestPlantedPosition(&(plant->PositionX), &(plant->PositionY), msg.x, msg.y);
				if (hasPlanted[plant->PositionX][plant->PositionY])
				{
					free(plant);
				}
				else
				{
					if (Head.nextOne == NULL && Head.lastOne == NULL)
					{
						Head.nextOne = plant;
						Head.lastOne = plant;
					}
					else
					{
						Head.lastOne->nextOne = plant;
						Head.lastOne = plant;
					}
					hasPlanted[plant->PositionX][plant->PositionY] = true;
				}	
			}
		}
	}
}

int getBestPlantedPosition(short *x, short *y,short mouseX,short mouseY) {

	
	for (int i = 0; i < 8; i++)
	{
		if (mouseX > GrassPositionX[i] - 37 && mouseX < GrassPositionX[i + 1] - 37) {
			*x = i;
			break;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (mouseY > GrassPositionY[i] - 37 && mouseY < GrassPositionY[i + 1] - 37) {
			*y = i;
			break;
		}
	}

	//printf("x= %d y = %d\n", *x, *y);
	return 1;
}

void log(const char* info) {
	printf("[INFO] %s \n",info);
}

void err(const char* info) {
	printf("[ERROR] %s \n", info);
}

char* getTitle() {
	sprintf(title,"%s %s", gameName,version);
	return title;
}
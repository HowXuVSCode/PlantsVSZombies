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
	//������Ϸ��Դ
	loadimage(&background, (LPCTSTR)backgroundPath);
	loadimage(&baseBar,(LPCTSTR) baseBarPath);
	//��Ϊ����ԭ������Ӧ�ö�1 ���ؿ���
	for (int i = 0; i < 3; i++)
	{
		char path[32];
		sprintf(path, "res/cards/card_%d.png", i+1);
		loadimage(&(cards[i]),(LPCTSTR)path);
	}
	//����ֲ�￨�Ʒ�֡
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

	//�������ڴ�СΪ640x480����
	HWND mainWindow = initgraph(900, 600,1); //����1����������̨����
	//_getch();
	if (mainWindow == NULL)
	{
		err("mainWindow create failed !");
		return -1;
	}
	//���ô��ڱ���
	if (!SetWindowText(mainWindow,(LPCTSTR) getTitle()))
	{
		err("mainWindow title change failed !");
	}
	return 0;
}

void GameUpdate() {

	//��Ե�һ�����庯���������޵���
	BeginBatchDraw();//��ʼ����
	//ÿ֡���±���ͼ Ӧ��initialize֮�����
	//ǰ����������������Ⱦλ��
	putimage(0, 0, &background);
	putimagePNG(250,0,&baseBar);
	//����ֲ�￨��
	for (int i = 0; i < cardNum; i++)
	{
		putimagePNG(CardBoxLeftPostion + CardWidth * i, CardBoxTopPostion, &cards[i]);
	}

	//�ж���괦���³�ֲ��
	if (isPaintPlantOnMouse && MouseX != 0 && MouseY != 0)
	{
		putimagePNG(MouseX - 37, MouseY - 37, &(plants[index][fps]));//֡������
	}

	struct Plant *toBePlanted = nullptr;
	if (Head.nextOne != nullptr && Head.lastOne != nullptr)
	{
		toBePlanted = Head.nextOne;
	}

	if (toBePlanted != nullptr)
	{
		do {
			//��Ⱦ��ֲ��ֲ��
			if (toBePlanted->index == 2)
			{
				//���컨�����ж�
				putimagePNG(GrassPositionX[toBePlanted->PositionX] - 35, GrassPositionY[toBePlanted->PositionY] - 60, &(plants[toBePlanted->index][fps]));
			}
			else
			{
				putimagePNG(GrassPositionX[toBePlanted->PositionX] - 30, GrassPositionY[toBePlanted->PositionY] - 40, &(plants[toBePlanted->index][fps]));
			}
			
			//�жϽ�����һ����Ⱦ
			if (toBePlanted->nextOne == nullptr)
			{
				break;
			}
			toBePlanted = toBePlanted->nextOne;
		} while (1);
	}

	EndBatchDraw();//��������

	//FPS����
	FPS_Control();
	
}

void FPS_Control() {
	//FPS����

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
	//��������
	return 0;
}



void onClick() {
	
	ExMessage msg;
	if (peekmessage(&msg)) {
		//�������仯
		//�����������ֲ�￨��
		if (msg.message == WM_LBUTTONDOWN)
		{
			
			if (msg.x > CardBoxLeftPostion && msg.x < CardBoxRightPostion && msg.y > CardBoxTopPostion && msg.y < CardBoxButtomPostion)
				//�ж�λ�ڿ���
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
			//�ɿ����
			MouseX = 0;
			MouseY = 0;
			isPaintPlantOnMouse = false;
			//�ж��Ƿ�Ϊ��ֲλ��
			if (msg.x > GrassLeftPositon && msg.y < GrassbuttomPositon && msg.y > GrassTopPositon)
			{
				//mallocһ��ֲ��ṹ��洢λ�ú�index������ʹ��EmptyPlant�궨
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
#pragma warning(disable : 4996) //关闭VS安全检查

#include <easyX.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

//GameName
#define version "1.0"
#define gameName "PlantsVSZombie"
#define backgroundPath "res/background.jpg"
#define baseBarPath "res/baseBar.png"
#define cardNum 10
#define plantsNum 3
#define FPSnum 13

#define CardBoxLeftPostion  335
#define CardBoxRightPostion  835
#define CardBoxTopPostion  5
#define CardBoxButtomPostion  92
#define CardWidth 65

#define GrassLeftPositon 256
#define GrassTopPositon 175
#define GrassbuttomPositon 490

//char gameName[16] = ;
//char *title = ;


//Log function
void log(const char* info);
void err(const char* info);

//Game
int GameInitialize();
int GameStop();
void GameUpdate();
void onClick();
void FPS_Control();
int getBestPlantedPosition(short *x, short *y, short mouseX, short mouseY);

char *getTitle();
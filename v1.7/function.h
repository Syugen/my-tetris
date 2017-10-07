#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "Mmsystem.h"
#pragma comment(lib, "Winmm.lib")//添加音乐

	//有棋子?		灰色棋子? 继续游戏? 悔棋过?	 买了东西? 任意棋子? 可爱橡皮? 强力橡皮?
bool Has[89]={0}, Grey[89]={0}, play=1, usehuiqi=0, buy=0, wheel=0;//, eraserA=0, eraserB=0;
	//剩余棋子	  关卡		分数	难度  可悔棋次数 改变棋子色    棋子原色	   下个棋子			当前棋子
int pieceleft=15, level=1, score=0, dif=1, huiqi=0, RGBC=15539236, RGBC_=RGBC, NextBlock=rand()%7, NewBlock=NextBlock;

int co=0, left=0, top=0, right, bottom, co_=0, left_, top_, right_, bottom_,TypeBlock=0;
char player[15]={0}; 
int i,j;
char slevel[3], spiece[3], sscore[8], str[50];

int test;

IMAGE icons, button, playerback;
IMAGE block, blockgrey, blockblue, blockred,blockredtemp;
IMAGE map, back, back2, backsml, backmo;
IMAGE message, messagesml, close;
IMAGE colorimg, tool, toolseven;

MOUSEMSG m;

void load()
{
	srand((unsigned)time(NULL));  
	mciSendString("open rc\\click.wav	alias click"	, NULL, 0, NULL);
	mciSendString("open rc\\clickkb.wav alias clickkb"	, NULL, 0, NULL);
	mciSendString("open rc\\error.wav	alias error"	, NULL, 0, NULL);
	mciSendString("open rc\\info.wav	alias info"		, NULL, 0, NULL);
	mciSendString("open rc\\stone.wav	alias stone"	, NULL, 0, NULL);
	mciSendString("open rc\\score.mp3	alias score"	, NULL, 0, NULL);
	mciSendString("open rc\\levelup.mp3 alias levelup"	, NULL, 0, NULL);
	mciSendString("open rc\\dead.mp3	alias dead"		, NULL, 0, NULL);
	mciSendString("open rc\\win.mp3		alias win"		, NULL, 0, NULL);
	loadimage(&map,			"rc\\map.jpg");
	loadimage(&button,		"rc\\button.jpg", 80, 32);
	loadimage(&message,		"rc\\message.jpg");
	loadimage(&blockgrey,	"rc\\blockgrey.jpg");
	loadimage(&blockblue,	"rc\\blockblue.jpg");
	loadimage(&blockred,	"rc\\blockred.jpg");
	loadimage(&blockredtemp,"rc\\blockred.jpg");
	loadimage(&block,		"rc\\block.jpg");
	loadimage(&icons,		"rc\\icons.jpg");
	loadimage(&colorimg,	"rc\\color.jpg");
	loadimage(&message,		"rc\\message.jpg");
	loadimage(&messagesml,	"rc\\messagesml.jpg");
	loadimage(&tool,		"rc\\tool.jpg",340,850);
	loadimage(&toolseven,	"rc\\toolseven.jpg",200,170);
	RGBC=BGR(RGBC);RGBC_=BGR(RGBC_);
}
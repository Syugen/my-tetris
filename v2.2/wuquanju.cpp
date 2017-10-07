// keshe.cpp : Defines the entry point for the console application.
//
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "Mmsystem.h"
#pragma comment(lib, "Winmm.lib")//添加音乐
#include "stdafx.h"

struct INFO
{
	int level;
	int score;
	int pieceleft;
	int dif;
	int huiqi;
	int RGBC;
	int RGBC_;
}nf;

#include "Click.h"
#include "Save.h"
#include "Block.h"

void load();
bool Game(char player[],bool read); 
void ShowRank(char player[]);
void ShowHelp();
int ClickPlay(bool Has[],bool Grey[],INFO nf,int NewBlock,char player[],bool *wheel);

void main()
{
	IMAGE map;
	loadimage(&map,	"rc\\map.jpg");
	char player[15]={0};
	bool LoggedIn=0,play=1,read=0;
	load();//初始化
	read=SaveReadClose();//读取存档
	while(!read && !LoggedIn)//如果没记录
	{
		if(ClickWelcome()) //欢迎界面
		{
			if(SaveLogIn(player))play=LoggedIn=1;//玩家信息
			else putimage(0,0,640,520,&map,0,500);
		}
		else play=0;
	}
	if(play)//玩游戏
	{
		solidrectangle(-1, 499, 640, 520);
		while(Game(player,read));
	}

	cleardevice();
	settextstyle(80, 0, "黑体"); 	settextcolor(WHITE);
	outtextxy(220, 200, "再见！");	Sleep(1000);
    closegraph();
}

bool Game(char player[],bool read)
{
	int i;
	FILE *fp=fopen("rc\\temp.save" , "w"); fclose(fp); //首先清空临时文件
	IMAGE back,map,playerback,difback,button, blocks,blockred;
	loadimage(&blockred,"rc\\blockred.jpg");
	loadimage(&blocks,"rc\\blocks.jpg");
	loadimage(&map,	"rc\\map.jpg");
	loadimage(&button,"rc\\button.jpg", 80, 32);
	
	bool Has[89]={0},Grey[89]={0};
	nf.RGBC=15539236, nf.RGBC_=nf.RGBC;
	nf.RGBC=BGR(nf.RGBC);nf.RGBC_=BGR(nf.RGBC_);
	nf.level=1;nf.score=0;nf.pieceleft=15;nf.huiqi=0;nf.dif=1;
	int NextBlock=rand()%7, NewBlock=NextBlock;
	bool buy=0,usehuiqi=0,wheel;//是否购买道具
	
	putimage(0,0,640,500,&map,0,0);
	getimage(&playerback, 168, 55, 150, 20);
	getimage(&difback, 378, 55, 50, 20);
	putimage(450, 7, &button); 					putimage(450, 40, &button);
	putimage(540, 7, &button); 					putimage(540, 40, &button);
	putimage(450, 222, &button);				putimage(540, 222, &button);
	settextcolor(WHITE); 						settextstyle(36, 0, "黑体");
	outtextxy(50, 15, "可爱的方块拼图小游戏");
	settextstyle(16, 0, "宋体");				outtextxy(470, 230, "悔棋       商店"); 
	outtextxy(55, 58, "现在的玩家是：");		outtextxy(170, 58, player);
	outtextxy(335, 58, "难度：");
	outtextxy(465, 15, "开局(R)    设置(S)");	outtextxy(465, 46, "帮助(H)    排名(L)");
	settextcolor(BLACK); 						outtextxy(470, 110, "级别:");
	outtextxy(470, 150, "金币(分数):"); 		outtextxy(470, 190, "需放棋子数:"); 
	
	if(read)//读取存档
	{
		FILE *fp=fopen("rc\\close.save", "r");
		fscanf(fp, "%s");
		fscanf(fp, "%d %d %d %d", &nf.level, &nf.score, &nf.pieceleft, &nf.dif);
		for(i=11; i<89; i++)
		{
			fscanf(fp, "%d %d", &Has[i], &Grey[i]);
			if(i%10==8){
				int left=6+54*(i%10-1),		top=84+52*(i/10-1);
				if(Grey[i])					putimage(left-1, top-1, 49,49,&blocks,0,50);
				else if(Has[i] && !Grey[i])	putimage(left-1, top-1, &blockred);
				else						putimage(left-1, top-1, 49,49,&blocks,0,0);}}
		fclose(fp);	
		fp=fopen("rc\\close.save" , "w"); fclose(fp); //清空记录文件
	}

	while(1)//每进行一次游戏操作执行一次																	//4.每次操作------//////
	{
		int i,j;
		BlockColor(&blockred, nf.RGBC,nf.RGBC_);
		putimage(575,100,57,121, &map,575,100);
		char slevel[3], spiece[3], sscore[8], sdif[5];
		sprintf(slevel, "%d", nf.level); 				outtextxy(580, 110, slevel);
		sprintf(sscore, "%d", nf.score); 				outtextxy(580, 150, sscore);
		sprintf(spiece, "%d", nf.pieceleft); 			outtextxy(580, 190, spiece);
		if(nf.dif==1)strcpy(sdif,"简单");
		else if(nf.dif==2)strcpy(sdif,"中等");
		else if(nf.dif==3)strcpy(sdif,"困难");
		settextcolor(WHITE);
		outtextxy(380, 58, sdif);
		settextcolor(BLACK);

		for(i=11; i<89; i++)//每次需要先根据Has和Grey把屏幕重新载入一遍
		{
			int left=6+54*(i%10-1), top=84+52*(i/10-1);
			if(Grey[i])putimage(left-1, top-1, 49,49,&blocks,0,50);
			else if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
			else putimage(left-1, top-1, 49,49,&blocks,0,0);
			if(i%10==8)i+=2; //防止出现19, 20等
		}

		if(!nf.pieceleft)//过关或第一次打开的情况														//4.3开局/关初始化-------//
		{	
			if(!nf.pieceleft){																					//4.3.1过关
				time_t start=time(NULL), finish=time(NULL);
				mciSendString("play levelup from 0", NULL, 0, NULL);
				ClickMSGSml("过关", "恭喜！您过关了！！！\n请等待音乐结束~", 2);
				while(finish-start<5.5)finish=time(NULL);
				nf.level++; nf.huiqi=0; }
			nf.pieceleft=15+5*(nf.level-1); //每一关剩余棋子数计算方法，可更改(注意：悔棋时用到本句话)

			if(nf.level==11){																					//4.3.2通关
				mciSendString("play win from 0", NULL, 0, NULL);
				ClickMSGSml("通关", "大神，您通关了~~", 2);
				SaveScore(nf.score,player);  ShowRank(player);  return 1; }

			putimage(575, 100, 57,121,&map,575,100); 	outtextxy(580, 150, sscore);
			sprintf(slevel, "%d", nf.level); 		outtextxy(580, 110, slevel);
			sprintf(spiece, "%d", nf.pieceleft); 	outtextxy(580, 190, spiece);
			
			double grey; //grey灰棋子概率
			if		(nf.dif==3)grey=(49+2*(nf.level-1))*0.01; //灰色棋子算法，可更改
			else if (nf.dif==2)grey=(34+2*(nf.level-1))*0.01;
			else if (nf.dif==1)grey=(19+2*(nf.level-1))*0.01;
			
			for(i=11; i<89; i++){																		//4.3.3生成灰色
				int left=6+54*(i%10-1), top=84+52*(i/10-1); j=5;
				if(Has[i] && !Grey[i]){
					while(j--){
						rectangle(left, top, left+46, top+46); Sleep(10);
						putimage(left-1, top-1, &blockred); Sleep(10);	}
					if(!(rand()%(int)(1/grey)))//上局遗留的部分转化为灰色
					{
						Has[i]=1; Grey[i]=1;
						mciSendString("play stone from 0", NULL, 0, NULL);
						putimage(left-1, top-1, 49,49,&blocks,0,50); //放上灰色
					}
					else {Has[i]=0; putimage(left-1, top-1, 49,49,&blocks,0,0); }}//上局遗留的没成灰色的
				//剩余情况：Has&&Grey（原本就是灰色）和!Has（原本空格）都不需处理
				if(i%10==8)i+=2;}
		}//过关或第一次打开的情况结束

		if(!buy){
			NewBlock=NextBlock; 
			NextBlock=rand()%7;	}
		else buy=0;

		if(!wheel){
			FILE *fp=fopen("rc\\temp.save", "a+"); 
			fprintf(fp, "\n%s %d %d %d %d ", player, nf.level, nf.score, nf.pieceleft, NewBlock);
			for(i=11; i<89; i++)fprintf(fp, "%d %d ", Has[i], Grey[i]);
			fclose(fp);}

		BlockDraw(Has,Grey,nf,NextBlock,0,0); //填写坐标为0表示要画出next
		
		int CanPut=BlockCanPut(Has,Grey,NewBlock);
		if(CanPut==0){
			SaveScore(nf.score,player); 
			char str[50];sprintf(str,"囧，您失败了……您的得分是%d。\n是否重玩？",nf.score);
			if(ClickMSGSml("游戏结束", str,3))
				{ShowRank(player); return 1;}
			else {ShowRank(player); return 0; }}
		if(!strcmp(player,"computer")){
			BlockDraw(Has,Grey,nf,NewBlock,0,CanPut);
			BlockDraw(Has,Grey,nf,NewBlock,1,CanPut);
			BlockDraw(Has,Grey,nf,NewBlock,2,CanPut);
			BlockDraw(Has,Grey,nf,NewBlock,3,CanPut);
			BlockDel(Has,Grey,&nf.score);
			Sleep(200);
			BlockDel(Has,Grey,&nf.score);nf.score+=(nf.dif*2-1);nf.pieceleft--; nf.huiqi++;continue;}

reput:
		switch(ClickPlay(Has,Grey,nf,NewBlock,player,&wheel))//对操作进行处理
		{
			case 0: if(ClickMSGSml("提示", "是否保存游戏记录？", 3))	//4.6.1退出
					{
						FILE *fp=fopen("rc\\close.save", "w"); //保存每次点击记录
						fprintf(fp, "%s %d %d %d %d ", player, nf.level, nf.score, nf.pieceleft, nf.dif);
						for(i=11; i<89; i++)fprintf(fp, "%d %d\n", Has[i], Grey[i]);
						fclose(fp);
					}
					return 0;
			case 1: BlockDel(Has,Grey,&nf.score);nf.score+=(nf.dif*2-1);nf.pieceleft--; nf.huiqi++;	 break; //4.6.2添加棋子
			case 91:																						//4.6.5点击重玩
				if(ClickMSGSml("重玩","确认重玩？",3)){return 1;}
				goto reput;
			case 92:																						//4.6.6点击设置
		//		{char show[200];sprintf(show,"%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d",Has[11],Has[12],Has[13],Has[14],Has[15],Has[16],Has[17],Has[18],Has[21],Has[22],Has[23],Has[24],Has[25],Has[26],Has[27],Has[28],Has[31],Has[32],Has[33],Has[34],Has[35],Has[36],Has[37],Has[38],Has[41],Has[42],Has[43],Has[44],Has[45],Has[46],Has[47],Has[48],Has[51],Has[52],Has[53],Has[54],Has[55],Has[56],Has[57],Has[58],Has[61],Has[62],Has[63],Has[64],Has[65],Has[66],Has[67],Has[68],Has[71],Has[72],Has[73],Has[74],Has[75],Has[76],Has[77],Has[78],Has[81],Has[82],Has[83],Has[84],Has[85],Has[86],Has[87],Has[88]);
		//		MessageBox(0,show,"",MB_OK);}
				int Set; getimage(&back, 0, 0, 640, 520);
				while(Set=ClickMSGSet(nf.dif))
				{
					if (Set==1)break;
					else if(Set==101)SaveChange(1,player); //改昵称
					else if(Set==102)SaveChange(2,player); //改密码，其实用不到player
					else if(Set==103)SaveDelete(player); //删用户
					else if(Set==104)SaveCreate(); //新用户	
					else if(Set==105){nf.dif=1;  }
					else if(Set==106){nf.dif=2;}
					else if(Set==107){nf.dif=3; }
					else if(Set==108){Set=ClickMSGRGB(); 
									if(Set!=1){	nf.RGBC_=nf.RGBC; nf.RGBC=Set; 
										BlockColor(&blockred, nf.RGBC,nf.RGBC_);break;}}
					if(Set==106||Set==107||Set==108){
						for(i=0;i<89;i++) Has[i]=Grey[i]=0;
						nf.level=1;nf.score=0;nf.pieceleft=15; 
						ClickMSGSml("提示", "游戏即将重新开局", 2);break;}
				}
				putimage(0, 0, &back);
				putimage(168, 55, &playerback); settextcolor(WHITE); //以下两行针对Set==101
				outtextxy(170, 58, player); settextcolor(BLACK);
				if(Set==106||Set==107||Set==108){putimage(378, 55, &difback); break;}
				for(i=11; i<89; i++){//本循环针对108
					int left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					if(i%10==8)i+=2;}
				goto reput;
			case 93:	getimage(&back, 0, 0, 640, 520);
						ShowHelp(); 			setfillcolor(RGB(45, 180, 160));
						putimage(0,0,&back);	goto reput;															//4.6.7点击帮助
			case 94: ShowRank(player); 	goto reput; 														//4.6.8点击排名
			case 95: if(nf.huiqi){//本关开始时不能悔棋														//4.6.9点击悔棋
				if(!usehuiqi){ClickMSGSml("提示", "每次悔棋扣5分！", 2); usehuiqi=1; }
				nf.huiqi--;
				SaveBack(Has,Grey,&nf,&NewBlock,player);
				putimage(575, 100, 57,121,&map,575,100);
				sprintf(slevel, "%d", nf.level); 		outtextxy(580, 110, slevel);
				sprintf(sscore, "%d", nf.score); 		outtextxy(580, 150, sscore);
				sprintf(spiece, "%d", nf.pieceleft); 	outtextxy(580, 190, spiece);
				for(i=11; i<89; i++)
				{
					int left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Grey[i])putimage(left-1, top-1, 49,49,&blocks,0,50);
					else if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					else putimage(left-1, top-1, 49,49,&blocks,0,0);
					if(i%10==8)i+=2;
				}}
				else ClickMSGSml("错误", "不能再悔棋了！", 1);
				goto reput;
			case 96: 
				getimage(&back, 0, 0, 640, 520);
				int Tool;
				while(Tool=ClickMSGTool())
				{
					if (Tool==1)break;
					else if(Tool==111 && nf.score>=500){nf.score-=501;nf.huiqi--;for(i=11;i<89;i++)if(Grey[i])Grey[i]=Has[i]=0;break;}
					else if(Tool==112 && nf.score>=200){nf.score-=201;nf.huiqi--;for(i=11;i<89;i++)Grey[i]=0;break;}
					else if(Tool==113 && nf.score>=50){nf.score-=51;wheel=1;nf.huiqi--;break;}
					else if(Tool==114 && nf.score>=150){nf.score-=51;NewBlock=7;break;}
					else if(Tool==115 && nf.score>=150){nf.score-=51;NewBlock=8;break;}
					else if(Tool==116 && nf.score>=150){nf.score-=101;NewBlock=9;break;}
					else ClickMSGSml("错误","金币不足",1);
				}
				putimage(0, 0, &back);buy=1;nf.huiqi++;continue;//由于重新循环存档多一次，需增加一次悔棋机会
		}
	}
}

void ShowRank(char player[])
{
	int i;
	IMAGE back,message;
	loadimage(&message,		"rc\\message.jpg");
	getimage(&back, 0, 0, 640, 520);
	ClickMosaic();
	putimage(100, 100, &message);
	outtextxy(104, 104, "排名(分数相等按照时间排序)");
	outtextxy(180, 130, "排名"); 	outtextxy(250, 130, "昵称" );
	outtextxy(400, 130, "分数");
	
	//生成链表
	user *H=NULL, *p=NULL, *file=NULL;
	FILE *fp=fopen("rc\\score.save", "r+");
	H=(user*)malloc(sizeof(user));     
	H->next=NULL;
	p=H;
	fscanf(fp, "\n");    
	while(!feof(fp))     
	{  
		file=(user*)malloc(sizeof(user));  
		fscanf(fp, "%s\t%d", file->name,&file->score);      
		file->next=NULL;//生成新结点         
		p->next=file;//插到尾部         
		p=file;//p后移         
	} 

	p=H->next;//输出
	char str[30];
	for(i=160; i<(160+10*20)&&p; i+=20, p=p->next)
	{
		sprintf(str, "%d", (i-140)/20);
		outtextxy(190, i, str);
		RECT r = {250, i, 440, i+20};
		drawtext(p->name, &r, DT_SINGLELINE);
		sprintf(str, "%d", p->score);
		outtextxy(400, i, str );
	}

	p=H->next;//查找自己
	int rank=1;
	for(; p&&strcmp(player, p->name); p=p->next)rank++;
	if(!p)rank=0;
	sprintf(str, "%s，您的排名是：%d", player, rank);
	outtextxy(200, 370, str );

	solidrectangle(-1, 499, 640, 520);
	outtextxy(5, 502, "提示：点击右上角x关闭，或者在键盘按回车或Esc。");
	ClickMSGRank();
	putimage(0, 0, &back);
	setfillcolor(RGB(45, 180, 160));
}

bool ShowHelpSleep(int time)
{
	MOUSEMSG m;
	IMAGE message;
	loadimage(&message,		"rc\\message.jpg");
	int start=clock(),end=clock(); 
	setfillcolor(RGB(45, 180, 160));
	while(end-start<time){
		if(kbhit())	{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())	{
				case 13:case 27:setfillcolor(WHITE);return 1;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;}}
		else if(MouseHit())	{	m = GetMouseMsg();
			if(m.x>528 && m.x<550 && m.y>100 && m.y<122){
				solidrectangle(-1,499,640,520); outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				rectangle(528,101,548,121);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);setfillcolor(WHITE);return 1;}}
			else{
				solidrectangle(-1,499,640,520);
				outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));
				putimage(527,100,23,23,&message,427,0);}}
		end=clock();}
	setfillcolor(WHITE);
	return 0;
}

bool ShowHelpTextFlash(char help[])
{
	solidrectangle(159,134,465,154);
	char helpchar[5];int i=0;
	while(i<(signed)strlen(help)){
		sprintf(helpchar,"%c%c",help[i],help[i+1]);
		outtextxy(160+8*i,135,helpchar);	if(ShowHelpSleep(50))return 1;	i+=2;}
	if(ShowHelpSleep(500))return 1;
	return 0;
}

void ShowHelp()
{	
	IMAGE help,message;
	loadimage(&message,		"rc\\message.jpg");
	loadimage(&help,"rc\\help.jpg");
	
	ClickMosaic();
	putimage(100, 100, &message);
	outtextxy(104, 104, "帮助");putimage(101,124,448,274,&help,0,0);
	int i=4; setfillcolor(WHITE);
	while(i--){
		putimage(161,174,274,70,&help,  0,275);		putimage(191,254,274,70,&help,  0,355);
		if(ShowHelpSleep(500))return;
		putimage(161,174,274,70,&help,280,275);		putimage(191,254,274,70,&help,280,355);
		if(ShowHelpSleep(500))return;}
	solidrectangle(161,174,465,324);
	if(ShowHelpTextFlash("游戏开始时有一个棋盘。"))return;
	putimage(200,175,217,210,&help,  0, 439);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("同时一个浅色的棋子将会跟随您的鼠标。"))	return;
	putimage(200,175,217,210,&help,216, 439);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("您可也用键盘方向键控制位置。"))			return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("鼠标点击左键或在键盘上按回车放置棋子。"))	return;
	putimage(200,175,217,210,&help,  0, 647);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("根据游戏难度每放一个棋子得到不同加分。"))	return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("简单加一分，中等加三分，困难加五分。"))	return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("继续这样操作。"))							return;
	putimage(200,175,217,210,&help,216, 647);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,  0, 855);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("当３ｘ３面积内充满棋子时它们将被消除。"))	return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("并且得到消除面积对应的额外金币。"))		return;
	putimage(200,175,217,210,&help,216, 855);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,  0,1063);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,216,1063);if(ShowHelpSleep(1500))return;
	solidrectangle(161,174,465,384);
	if(ShowHelpTextFlash("充满大于３ｘ３面积也会消除。"))			return;
	if(ShowHelpTextFlash("但如果同时以下情况："))					return;
	putimage(200,175,217,210,&help,  0,1271);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("两块可消除区域有公共部分。"))				return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("将消除面积更大的区域。"))					return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("如果面积相等优先消除瘦高的。"))			return;
	putimage(200,175,217,210,&help,216,1271);if(ShowHelpSleep(1500))return;
	solidrectangle(161,174,465,384);
	if(ShowHelpTextFlash("鼠标右键或键盘空格可以旋转棋子。"))		return;
	putimage(200,175,217,210,&help,  0,1479);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,216,1479);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,  0,1479);if(ShowHelpSleep(1500))return;
	solidrectangle(161,174,465,384);
	if(ShowHelpTextFlash("当“需放棋子数”为零时过关。"))			return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("此时未被消除的棋子按一定概率变灰。"))		return;if(ShowHelpSleep(500))return;
	putimage(200,175,217,210,&help,  0,1687);if(ShowHelpSleep(1500))return;
	putimage(200,175,217,210,&help,216,1687);if(ShowHelpSleep(1500))return;
	if(ShowHelpTextFlash("变为灰色棋子后它们将不能被消除。"))		return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("游戏越难变灰概率越大，概率也随关卡变大"))	return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("如果棋盘上放不下更多的棋子则游戏结束。"))	return;if(ShowHelpSleep(500))return;
	if(ShowHelpTextFlash("游戏还有超多功能待您去探索（懒得写了）"))	return;if(ShowHelpSleep(500))return;
	solidrectangle(161,174,465,384);
	while(1){
		putimage(161,174,274,70,&help,  0,275);		putimage(191,254,274,70,&help,  0,355);
		if(ShowHelpSleep(500))return;
		putimage(161,174,274,70,&help,280,275);		putimage(191,254,274,70,&help,280,355);
		if(ShowHelpSleep(500))return;}
}

int ClickPlay(bool Has[],bool Grey[],INFO nf,int NewBlock,char player[],bool *wheel)
{
	MOUSEMSG m;
	IMAGE backsml,blocks,blockred;
	loadimage(&blockred,"rc\\blockred.jpg");
	loadimage(&blocks,"rc\\blocks.jpg");
	getimage(&backsml, 6, 84, 424, 410); //每次获取最大可能的范围
	BlockColor(&blockred, nf.RGBC,nf.RGBC_);
	
	int co=44, codown, left, top, right, bottom, co_=0, left_, top_, right_, bottom_;
	left_=left=168; top_=top=240; right_=right=214; bottom_=bottom=286; //为键盘设计，初始格子是4排4列
	int i,TypeBlock=0;

reput:
	while(true)																								//////5每无效操作一次循环一次(如鼠标移动,无效点击)
	{
		if(kbhit())																							////5.1键盘操作-------//
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			int get=getch();
			if	   (get=='R' || get=='r')return 91;
			else if(get=='S' || get=='s')return 92;
			else if(get=='H' || get=='h')return 93;
			else if(get=='L' || get=='l')return 94;
			else if(get==13)
			{
				solidrectangle(left, top, right, bottom);
				Sleep(100);
				putimage(left-1, top-1, 49,49,&blocks,0,0);
				if(!BlockDraw(Has,Grey,nf,NewBlock,TypeBlock,co)){
					putimage(6,84,&backsml);
					mciSendString("play error from 0", NULL, 0, NULL);
					goto reput;}
				getimage(&backsml,6,84,430,494);
				return 1;
			}
			else if(get==27) return 0;
			else if(get==' ')//旋转
			{
				TypeBlock++;
				if(TypeBlock==4)TypeBlock=0;
				putimage(6, 84, &backsml);
				getimage(&backsml, 6, 84, 424, 410);
				BlockTemp(Has,nf.RGBC,nf.RGBC_,NewBlock,TypeBlock,co); //画出临时图
			}
			else if(get==0xE0)//小键盘上下左右
			{
				switch(getch())
				{
					case 72:if(top>84){co-=10; top-=52; bottom-=52; }break;
					case 75:if(left>6){co-= 1; left-=54; right-=54; }break;
					case 77:if(right<430){co+= 1; left+=54; right+=54; }break;
					case 80:if(bottom<494){co+=10; top+=52; bottom+=52; }break;
				}
				if(co!=co_&&co_)putimage(6,84,&backsml);//恢复上一格子边框颜色
				BlockTemp(Has,nf.RGBC,nf.RGBC_,NewBlock,TypeBlock,co); //画出临时图
				co_=co; left_=left; top_=top; right_=right; bottom_=bottom;
				continue;
			}
			else mciSendString("play error from 0", NULL, 0, NULL); //break;
		}
		else if(MouseHit())																					////5.2鼠标操作-------//
		{
			m = GetMouseMsg();
			if(m.x>=  6 && m.x<=430&&m.y>= 84 && m.y<=494)													//5.2.1在格子或格子间隙里
			{
				if	   (m.x>=  3 && m.x<= 56){co = 1; left=  6; right= 52; }//6-52横向间距54, 每格46, 此处算上了4像素边框
				else if(m.x>= 57 && m.x<=110){co = 2; left= 60; right=106; }//60-106
				else if(m.x>=111 && m.x<=164){co = 3; left=114; right=160; }//114-160
				else if(m.x>=165 && m.x<=218){co = 4; left=168; right=214; }//168-214
				else if(m.x>=219 && m.x<=272){co = 5; left=222; right=268; }//222-268
				else if(m.x>=273 && m.x<=326){co = 6; left=276; right=322; }//276-322
				else if(m.x>=327 && m.x<=380){co = 7; left=330; right=376; }//330-376
				else if(m.x>=381 && m.x<=434){co = 8; left=384; right=430; }//384-430
				if	   (m.y>= 82 && m.y<=133){co+=10; top= 84; bottom=130; }//84-136纵向间距52, 每格46, 算上边框3像素
				else if(m.y>=134 && m.y<=185){co+=20; top=136; bottom=182; }//136-182
				else if(m.y>=186 && m.y<=237){co+=30; top=188; bottom=234; }//188-234
				else if(m.y>=238 && m.y<=289){co+=40; top=240; bottom=286; }//240-286
				else if(m.y>=290 && m.y<=341){co+=50; top=292; bottom=338; }//292-338
				else if(m.y>=342 && m.y<=393){co+=60; top=344; bottom=390; }//344-390
				else if(m.y>=394 && m.y<=445){co+=70; top=396; bottom=442; }//396-442
				else if(m.y>=446 && m.y<=497){co+=80; top=448; bottom=494; }//448-494
				
				if(m.uMsg==WM_RBUTTONUP){//旋转
					TypeBlock++;
					if(TypeBlock==4)TypeBlock=0;
					putimage(6, 84, &backsml);
					getimage(&backsml, 6, 84, 424, 410);}
				if(m.uMsg==WM_MOUSEWHEEL && *wheel){
					putimage(6,84,&backsml);
					NewBlock=NewBlock>=6?0:(NewBlock+1);}
				if(co!=co_&&co_)putimage(6,84,&backsml);//鼠标不在同一个格子里时恢复上一格子边框颜色
				if(m.uMsg==WM_LBUTTONDOWN){
					solidrectangle(left, top, right, bottom); //点下时框框变绿
					codown=co;}

				BlockTemp(Has,nf.RGBC,nf.RGBC_,NewBlock,TypeBlock,co); //画出临时图
				co_=co; left_=left; top_=top; right_=right; bottom_=bottom;
				
				if(m.uMsg==WM_LBUTTONUP && codown==co)//鼠标点击和抬起在同一框内才能放置棋子
				{
					if(!BlockDraw(Has,Grey,nf,NewBlock,TypeBlock,co)){
						putimage(6,84,&backsml);
						mciSendString("play error from 0", NULL, 0, NULL);
						goto reput;} //没添加上重新来
					mciSendString("play click from 0", NULL, 0, NULL);
					getimage(&backsml,6,84,430,494);
					*wheel=0;//无论买没买任意棋子，到这里都失效
					return 1;
				}
			}

			else if(m.x>470 && m.x<540 && m.y>7 && m.y<39){													//5.2.2移动到开始按钮
				solidrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击重新开始游戏，或者在键盘按“R”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 91; }}
			else if(m.x>550 && m.x<620 && m.y>7 && m.y<39){													//5.2.3移动到设置按钮
				solidrectangle(-1, 499, 640, 520);  outtextxy(5, 502, "提示：点击设置游戏，或者在键盘按“S”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 92; }}
			else if(m.x>470 && m.x<540 && m.y>40 && m.y<72){												//5.2.4移动到帮助按钮
				solidrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击显示游戏帮助，或者在键盘按“H”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 93; }}
			else if(m.x>550 && m.x<620 && m.y>40 && m.y<72){												//5.2.5移动到排名按钮
				solidrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击显示玩家排名，或者在键盘按“L”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 94; }}
			else if(m.x>450 && m.x<530 && m.y>222 && m.y<254){												//5.2.6移动到悔棋按钮
				solidrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击退回一步，悔棋一步扣除五分。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 95; }}
			else if(m.x>540 && m.x<620 && m.y>222 && m.y<254){												//5.2.6移动到悔棋按钮
				solidrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击购买道具。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 96; }}
			else {																							//5.2.6移动到空区域
				for(i=11; i<89; i++)//取消棋子预览
				{
					left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Grey[i])putimage(left-1, top-1, 49,49,&blocks,0,50);
					else if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					else putimage(left-1, top-1, 49,49,&blocks,0,0);
					if(i%10==8)i+=2; //防止出现19, 20等
				}
				solidrectangle(-1, 499, 640, 520); //移出区域后清除提示
				outtextxy(5, 502, "提示：退出游戏点击右上角x，或者在键盘按Esc。"); }
		}//鼠标点击结束
	}//while结束
}

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
	
	initgraph(640, 520);

	IMAGE map;
	loadimage(&map,	"rc\\map.jpg");
	putimage(0,0,640,520,&map,0,500); //初始界面
	setbkmode(TRANSPARENT); 						setlinecolor(RGB(45, 180, 160));
	setlinestyle(PS_JOIN_MITER|PS_SOLID, 3); 		setfillcolor(RGB(45, 180, 160));
	settextcolor(BLACK); 							settextstyle(16, 0, "宋体");
}

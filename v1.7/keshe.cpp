// keshe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "function.h"
#include "Save.h"
//#include "Click.h"
#include "Block.h"

//结构体已经在Save.h中定义

/*声明函数
void SaveRead(user *H, FILE *fp); 		//读取save生成链表，需要提前创建头指针、打开文件以便执行完毕后使用
void SaveNew(user *input); 				//询问新用户密码，将新用户信息存储在save后端
void SaveNodeDel(user *H); 				//删除链表，释放内存
bool SaveInputPass(user *p, user *input); //问用户密码，无密码或输入正确返回1，五次输错或取消返回0
bool SaveLogIn(); 						//登录系统，正常登录返回 1，取消登录返回 0
bool SaveCreate(); 						//新建用户，成功建立返回 1，取消建立返回 0
bool SaveDelete(); 						//删除用户，成功删除返回 1，取消建立返回 0
int SaveChange(int type);	 //type=1改昵称2改密码，修改成功返回 1，取消修改返回 0
bool ClickWelcome(); 			//欢迎界面点击判断，继续程序返回 1，退出程序返回 0
bool ClickMSG(); 			//帮助排名界面点击判断，继续程序返回 1
bool ClickMSGSml(int type);	  //所有小窗口信息界面，继续程序返回 1
int  ClickMSGSet(); 				//设置界面点击判断，继续程序返回 1，点击选项返回101-108
void BlockTemp(); 	//鼠标或键盘移动时画出临时的格子形状
int  BlockDraw(int NewNextB); //鼠标或键盘确定后画出棋子，放置成功返回1，放置失败返回0
void BlockDel(); 						//棋盘达到3x3以上消除
int  BlockCanPut(); 						//检验是否游戏结束，游戏未结束返回1，重玩返回2，到游戏初始界面返回3
*/
int Game(); //返回0结束，返回-2回到开始界面
void ShowRank();
int ClickPlay();
																//游戏界面点击判断，放棋子返回1，重玩返回2，到游戏初始界面返回3，退出返回0，点击选项返回91-95

void main()
{
	initgraph(640, 520);
start:	
	load();
	putimage(0,0,640,520,&map,0,500); //初始界面
	setbkmode(TRANSPARENT); 						setlinecolor(RGB(45, 180, 160));
	setlinestyle(PS_JOIN_MITER|PS_SOLID, 3); 		setfillcolor(RGB(45, 180, 160));
	settextcolor(BLACK); 							settextstyle(16, 0, "宋体");

	FILE *fp=fopen("rc\\close.save", "r");
	char fileplayer[15]={0};
	fscanf(fp, "%s", fileplayer);
	if(fileplayer[0])//如果有记录
	{
		sprintf(str, "是否读取%s的游戏记录？", fileplayer);
		if(ClickMSGSml("提示", str, 3))
		{
			fscanf(fp, "%d %d %d %d", &level, &score, &pieceleft, &dif);
			for(i=11; i<89; i++)fscanf(fp, "%d %d", &Has[i], &Grey[i]);
		}
		else strcpy(fileplayer, "\0");
		FILE *fp=fopen("rc\\close.save" , "w"); fclose(fp); //清空记录文件
	}
	fclose(fp);
	if(!fileplayer[0]) //如果没记录
	{
		while(!(play=ClickWelcome()))
			if(!play)goto stop;
		while((play=SaveLogIn())!=1)//玩家信息
			if(!play)goto start;
	}

	fillrectangle(-1, 499, 640, 520);
	if(Game()==3)goto start; //开始游戏

stop:
	cleardevice();
	settextstyle(80, 0, "黑体"); 	settextcolor(WHITE);
	outtextxy(220, 200, "再见！");	Sleep(1000);
    closegraph();
}

int Game()
{
restart:
	FILE *fp=fopen("rc\\temp.save" , "w"); fclose(fp); //首先清空临时文件	
	putimage(0,0,640,500,&map,0,0);
	while(play)//每进行一次游戏操作执行一次																	//4.每次操作------//////
	{
		putimage(0,0,640,500,&map,0,0);
		getimage(&playerback, 168, 55, 200, 20);										//4.1贴图------------//
		putimage(450, 7, &button); 					putimage(450, 40, &button);
		putimage(540, 7, &button); 					putimage(540, 40, &button);
		putimage(450, 222, &button);				putimage(540, 222, &button);
		settextcolor(WHITE); 						settextstyle(36, 0, "黑体");
		outtextxy(50, 15, "可爱的方块拼图小游戏");
		settextstyle(16, 0, "宋体");				outtextxy(470, 230, "悔棋       商店"); 
		outtextxy(55, 58, "现在的玩家是：");		outtextxy(170, 58, player);
		outtextxy(465, 15, "开局(R)    设置(S)");	outtextxy(465, 46, "帮助(H)    排名(L)");
		settextcolor(BLACK); 						outtextxy(470, 110, "级别:");
		outtextxy(470, 150, "金币(分数):"); 				outtextxy(470, 190, "需放棋子数:"); 	
		putimage(575,100,57,121, &map,575,100);
		sprintf(slevel, "%d", level); 				outtextxy(580, 110, slevel);
		sprintf(sscore, "%d", score); 				outtextxy(580, 150, sscore);
		sprintf(spiece, "%d", pieceleft); 			outtextxy(580, 190, spiece);

		for(i=11; i<89; i++)	{														//4.2贴棋子-----------//
			left=6+54*(i%10-1);			top=84+52*(i/10-1);
			if(Grey[i])					putimage(left-1, top-1, &blockgrey);
			else if(Has[i] && !Grey[i])	putimage(left-1, top-1, &blockred);
			else						putimage(left-1, top-1, &block);
			if(i%10==8)i+=2; }

		if(!pieceleft)//过关或第一次打开的情况														//4.3开局/关初始化-------//
		{	
			if(!pieceleft){																					//4.3.1过关
				time_t start=time(NULL), finish=time(NULL);
				mciSendString("play levelup from 0", NULL, 0, NULL);
				ClickMSGSml("过关", "恭喜！您过关了！！！\n请等待音乐结束~", 2);
				while(finish-start<5.5)finish=time(NULL);
				level++; huiqi=0; }
			pieceleft=15+5*(level-1); //每一关剩余棋子数计算方法，可更改(注意：悔棋时用到本句话)

			if(level==11){																					//4.3.2通关
				mciSendString("play win from 0", NULL, 0, NULL);
				ClickMSGSml("通关", "大神，您通关了~~", 2);
				SaveScore();  ShowRank();  return 3; }

			putimage(575, 100, 57,121,&map,575,100); 	outtextxy(580, 150, sscore);
			sprintf(slevel, "%d", level); 		outtextxy(580, 110, slevel);
			sprintf(spiece, "%d", pieceleft); 	outtextxy(580, 190, spiece);
			
			double grey; //grey灰棋子概率
			if		(dif==3)grey=(49+2*(level-1))*0.01; //灰色棋子算法，可更改
			else if (dif==2)grey=(34+2*(level-1))*0.01;
			else if (dif==1)grey=(19+2*(level-1))*0.01;
			
			for(i=11; i<89; i++){																		//4.3.3生成灰色
				left=6+54*(i%10-1); top=84+52*(i/10-1); j=5;
				if(Has[i] && !Grey[i]){
					while(j--){
						rectangle(left, top, left+46, top+46); Sleep(10);
						putimage(left-1, top-1, &blockred); Sleep(10);	}
					if(!(rand()%(int)(1/grey)))//上局遗留的部分转化为灰色
					{
						Has[i]=1; Grey[i]=1;
						mciSendString("play stone from 0", NULL, 0, NULL);
						putimage(left-1, top-1, &blockgrey); //放上灰色
					}
					else {Has[i]=0; putimage(left-1, top-1, &block); }}//上局遗留的没成灰色的
				//剩余情况：Has&&Grey（原本就是灰色）和!Has（原本空格）都不需处理
				if(i%10==8)i+=2;}
		}//过关或第一次打开的情况结束

		if(!buy){
			NewBlock=NextBlock; 																				//4.4随机出现棋子程序-------//
			NextBlock=rand()%7;
		}
		else buy=0;

		if(!wheel){
			FILE *fp=fopen("rc\\temp.save", "a+"); 																//4.5保存每次点击记录-------//
			fprintf(fp, "\n%s %d %d %d %d ", player, level, score, pieceleft, NewBlock);
			for(i=11; i<89; i++)fprintf(fp, "%d %d ", Has[i], Grey[i]);
			fclose(fp);}

reput:
		switch(ClickPlay())								//4.6对每次操作的判断
		{
			case 0: if(ClickMSGSml("提示", "是否保存游戏记录？", 3))	//4.6.1退出
					{
						FILE *fp=fopen("rc\\close.save", "w"); //保存每次点击记录
						fprintf(fp, "%s %d %d %d %d ", player, level, score, pieceleft, dif);
						for(i=11; i<89; i++)fprintf(fp, "%d %d\n", Has[i], Grey[i]);
						fclose(fp);
					}
					return 0;
			case 1: score+=(2*dif-1);pieceleft--; huiqi++;	 break; 															//4.6.2添加棋子
			case 2: ShowRank(); goto restart; 								//4.6.3失败后重玩
			case 3: ShowRank(); return 3; 									//4.6.4回到欢迎界面
			case 91:																						//4.6.5点击重玩
				if(ClickMSGSml("重玩","确认重玩？",3)){
					for(i=11;i<89;i++)Has[i]=Grey[i]=0;level=1;score=0;pieceleft=15;huiqi=0;goto restart;}
				goto reput;
			case 92:																						//4.6.6点击设置
				int Set; getimage(&back, 0, 0, 640, 520);
				while(Set=ClickMSGSet())
				{
					if (Set==1)break;
					else if(Set==101)SaveChange(1); //改昵称
					else if(Set==102)SaveChange(2); //改密码，其实用不到player
					else if(Set==103)SaveDelete(); //删用户
					else if(Set==104)SaveCreate(); //新用户	
					else if(Set==105){dif=1; ClickMSGSml("提示", "游戏即将重新开局", 2);
									for(i=11;i<89;i++)Has[i]=Grey[i]=0;level=1;score=0;pieceleft=15;huiqi=0;goto restart; }
					else if(Set==106){dif=2; ClickMSGSml("提示", "游戏即将重新开局", 2);
									for(i=11;i<89;i++)Has[i]=Grey[i]=0;level=1;score=0;pieceleft=15;huiqi=0;goto restart; }
					else if(Set==107){dif=3; ClickMSGSml("提示", "游戏即将重新开局", 2);
									for(i=11;i<89;i++)Has[i]=Grey[i]=0;level=1;score=0;pieceleft=15;huiqi=0;goto restart; }
					else if(Set==108){Set=ClickMSGRGB(); 
									if(Set!=1){	RGBC_=RGBC; RGBC=Set; 
										BlockColor(&blockred, RGBC);saveimage("c:\\temp.jpg",&blockredtemp);}}
				}
				putimage(0, 0, &back);
				putimage(168, 55, &playerback); settextcolor(WHITE); //本两句针对value==101情况
				outtextxy(170, 58, player); settextcolor(BLACK);
				for(i=11; i<89; i++){//本循环针对108
					left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					if(i%10==8)i+=2;}
				goto reput;
			case 93:																						//4.6.7点击帮助
		//		sprintf(str,"%d",test);
		//		ClickMSGSml("",str,1);
		//		{char show[200];sprintf(show,"%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d\n%d %d %d %d %d %d %d %d",Has[11],Has[12],Has[13],Has[14],Has[15],Has[16],Has[17],Has[18],Has[21],Has[22],Has[23],Has[24],Has[25],Has[26],Has[27],Has[28],Has[31],Has[32],Has[33],Has[34],Has[35],Has[36],Has[37],Has[38],Has[41],Has[42],Has[43],Has[44],Has[45],Has[46],Has[47],Has[48],Has[51],Has[52],Has[53],Has[54],Has[55],Has[56],Has[57],Has[58],Has[61],Has[62],Has[63],Has[64],Has[65],Has[66],Has[67],Has[68],Has[71],Has[72],Has[73],Has[74],Has[75],Has[76],Has[77],Has[78],Has[81],Has[82],Has[83],Has[84],Has[85],Has[86],Has[87],Has[88]);
		//		MessageBox(0,show,"",MB_OK);}
				getimage(&back, 0, 0, 640, 520);	getimage(&backmo,0,0,640,520);
				Mosaic(&backmo, 15, 0, 0);	putimage(0, 0, &backmo);  putimage(100, 100, &message);
				outtextxy(104, 104, "帮助");
				{char help[1000]={"游戏与计分规则：将出现的棋子放于棋盘上，当棋盘中出现3×3或更大面积的方块被放上棋子时，这个方块的棋子将被消掉。图中的灰色棋子可以辅助构成方阵，但是不能被消去。每在棋盘上放置一个棋子加一分，每次消掉棋子，消掉几个棋子加几分（灰色棋子也算分）。\n\n关卡设置：游戏共有10关，每关需要放置的棋子数=10+4×(Level-1)，每关灰色棋子数量大约=上一关结束时未被消掉的棋子数×(40+2×(Level-1))%。\n\n游戏结束：游戏共10关，全部过关即通关。如果棋盘放不下更多的棋子则游戏结束。\n\n操作说明：鼠标左键放置棋子，右键旋转棋子。键盘方向键控制位置，空格键旋转棋子，回车键放置棋子。"};
				RECT r = {110, 130, 530, 400}; 	drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS ); }
				fillrectangle(-1, 499, 640, 520); 	outtextxy(5, 502, "提示：点击右上角x关闭，或者在键盘按回车或Esc。");
				ClickMSG();
				putimage(0, 0, &back);
				goto reput;
			case 94: ShowRank(); 	goto reput; 														//4.6.8点击排名
			case 95: if(huiqi){//本关开始时不能悔棋														//4.6.9点击悔棋
				if(!usehuiqi){ClickMSGSml("提示", "每次悔棋扣5分！", 2); usehuiqi=1; }
				huiqi--;
				SaveBack();
				putimage(575, 100, 57,121,&map,575,100);
				sprintf(slevel, "%d", level); 		outtextxy(580, 110, slevel);
				sprintf(sscore, "%d", score); 		outtextxy(580, 150, sscore);
				sprintf(spiece, "%d", pieceleft); 	outtextxy(580, 190, spiece);
				for(i=11; i<89; i++)
				{
					left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Grey[i])putimage(left-1, top-1, &blockgrey);
					else if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					else putimage(left-1, top-1, &block);
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
					else if(Tool==111 && score>=500){score-=501;for(i=11;i<89;i++)if(Grey[i])Grey[i]=Has[i]=0;break;}
					else if(Tool==112 && score>=200){score-=201;for(i=11;i<89;i++)Grey[i]=0;break;}
					else if(Tool==113 && score>=50){score-=51;wheel=1;huiqi--;break;}
					else if(Tool==114 && score>=150){score-=51;NewBlock=7;break;}
					else if(Tool==115 && score>=150){score-=51;NewBlock=8;break;}
					else if(Tool==116 && score>=150){score-=101;NewBlock=9;break;}
					else ClickMSGSml("错误","金币不足",1);
				}
				putimage(0, 0, &back);buy=1;huiqi++;continue;//由于重新循环存档多一次，需增加一次悔棋机会
		}
	}
	return 0;
}

void ShowRank()
{
	getimage(&back, 0, 0, 640, 520);	getimage(&backmo,0,0,640,520);
	Mosaic(&backmo, 15, 0, 0);			putimage(0, 0, &backmo);  
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

	fillrectangle(-1, 499, 640, 520);
	outtextxy(5, 502, "提示：点击右上角x关闭，或者在键盘按回车或Esc。");
	ClickMSG();
	putimage(0, 0, &back);
}



int ClickPlay()
{
	getimage(&backsml, 6, 84, 424, 410); //每次获取最大可能的范围
	switch(test=BlockCanPut())																		//////2判断结束
	{
		case 2:return 2; //重玩
		case 3:return 3; //回到开始
		case 1:break;
	}

	BlockDraw(NextBlock,0); //填写坐标为0表示要画出next											//////4画出下个棋子
	int codown;
	co=44; left_=left=168; top_=top=240; right_=right=214; bottom_=bottom=286; //为键盘设计，初始格子是4排4列
	TypeBlock=0;
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
				fillrectangle(left, top, right, bottom);
				Sleep(100);
				putimage(left-1, top-1, &block);
				if(!BlockDraw(NewBlock,co))goto reput;
				BlockDel();//加分包含在其中
				return 1;
			}
			else if(get==27) return 0;
			else if(get==' ')//旋转
			{
				TypeBlock++;
				if(TypeBlock==4)TypeBlock=0;
				putimage(6, 84, &backsml);
				getimage(&backsml, 6, 84, 424, 410);
				BlockTemp(); //画出临时图
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
				BlockTemp(); //画出临时图
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
				if(m.uMsg==WM_MOUSEWHEEL && wheel){
					putimage(6,84,&backsml);
					NewBlock=NewBlock>=6?0:(NewBlock+1);}
				if(co!=co_&&co_)putimage(6,84,&backsml);//鼠标不在同一个格子里时恢复上一格子边框颜色
				if(m.uMsg==WM_LBUTTONDOWN){
					fillrectangle(left, top, right, bottom); //点下时框框变绿
					codown=co;}

				BlockTemp(); //画出临时图
				co_=co; left_=left; top_=top; right_=right; bottom_=bottom;
				
				if(m.uMsg==WM_LBUTTONUP && codown==co)//鼠标点击和抬起在同一框内才能放置棋子
				{
					mciSendString("play click from 0", NULL, 0, NULL);
					if(!BlockDraw(NewBlock,co))goto reput; //没添加上重新来
					wheel=0;//无论买没买任意棋子，到这里都失效
					BlockDel();
					return 1;
				}
			}

			else if(m.x>470 && m.x<540 && m.y>7 && m.y<39){													//5.2.2移动到开始按钮
				fillrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击重新开始游戏，或者在键盘按“R”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 91; }}
			else if(m.x>550 && m.x<620 && m.y>7 && m.y<39){													//5.2.3移动到设置按钮
				fillrectangle(-1, 499, 640, 520);  outtextxy(5, 502, "提示：点击设置游戏，或者在键盘按“S”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 92; }}
			else if(m.x>470 && m.x<540 && m.y>40 && m.y<72){												//5.2.4移动到帮助按钮
				fillrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击显示游戏帮助，或者在键盘按“H”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 93; }}
			else if(m.x>550 && m.x<620 && m.y>40 && m.y<72){												//5.2.5移动到排名按钮
				fillrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击显示玩家排名，或者在键盘按“L”。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 94; }}
			else if(m.x>450 && m.x<530 && m.y>222 && m.y<254){												//5.2.6移动到悔棋按钮
				fillrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击退回一步，悔棋一步扣除五分。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 95; }}
			else if(m.x>540 && m.x<620 && m.y>222 && m.y<254){												//5.2.6移动到悔棋按钮
				fillrectangle(-1, 499, 640, 520); outtextxy(5, 502, "提示：点击购买道具。");
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL); return 96; }}
			else {																							//5.2.6移动到空区域
				setlinecolor(WHITE);
				for(i=11; i<89; i++)//取消棋子预览
				{
					left=6+54*(i%10-1), top=84+52*(i/10-1);
					if(Grey[i])putimage(left-1, top-1, &blockgrey);
					else if(Has[i] && !Grey[i])putimage(left-1, top-1, &blockred);
					else putimage(left-1, top-1, &block);
					if(i%10==8)i+=2; //防止出现19, 20等
				}
				setlinecolor(RGB(45, 180, 160));
				fillrectangle(-1, 499, 640, 520); //移出区域后清除提示
				outtextxy(5, 502, "提示：退出游戏点击右上角x，或者在键盘按Esc。"); }
		}//鼠标点击结束
	}//while结束
}


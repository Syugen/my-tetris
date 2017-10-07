#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include "Mmsystem.h"
#pragma comment(lib,"Winmm.lib")//添加音乐

void Mosaic(IMAGE *pimg, int tilesize, int startx, int starty)
{
	int	width	= pimg->getwidth();		// 图像的宽
	int	height	= pimg->getheight();	// 图像的高
	int	redsum;			// 红色值的和
	int	greensum;		// 绿色值的和
	int	bluesum;		// 蓝色值的和
	int	count;			// 每个小方块内的像素数量
	int	color;			// 每个像素的颜色
	int x, y, tx, ty;	// 循环变量

	// 获取指向显存的指针
	DWORD* pMem = GetImageBuffer(pimg);

	// 求出左上角第一个方块的坐标
	startx = (startx % tilesize == 0 ? 0 : startx % tilesize - tilesize);
	starty = (starty % tilesize == 0 ? 0 : starty % tilesize - tilesize);

	// 处理每一个小方块
	for (y = starty; y < height; y += tilesize)
		for (x = startx; x < width; x += tilesize)
		{
			// 清空累加值
			redsum = greensum = bluesum = count = 0;

			// 求小方块的红、绿、蓝颜色值的和
			for (ty = min(y + tilesize, height) - 1; ty >= max(y, 0); ty--)
				for (tx = min(x + tilesize, width) - 1; tx >= max(x, 0); tx--)
				{
					color = pMem[ty * width + tx];
					redsum += GetRValue(color);
					greensum += GetGValue(color);
					bluesum += GetBValue(color);
					count++;
				}

			// 求红、绿、蓝颜色的平均值
 			redsum /= count;
			greensum /= count;
			bluesum /= count;

			// 设置小方块内的每个像素为平均颜色值
			color = RGB(redsum, greensum, bluesum);
			for (ty = min(y + tilesize, height) - 1; ty >= max(y, 0); ty--)
				for (tx = min(x + tilesize, width) - 1; tx >= max(x, 0); tx--)
					pMem[ty * width + tx] = color;
		}
}


bool ClickWelcome()
{
	while(true)
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())
			{
				case 13:return 1;//getch()中13为回车
				case 27:return 0;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;
			}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();
			if((m.x-175)*(m.x-175)+(m.y-405)*(m.y-405)<=2025)
			{
				if(m.uMsg==WM_LBUTTONDOWN)	putimage(119,355,109,102,&icons,0,0);
				else if(m.uMsg==WM_LBUTTONUP)
				{
					mciSendString("play click from 0", NULL, 0, NULL);
					loadimage(NULL,"rc\\start.jpg");
					return 1;
				}
			}
			else if(m.uMsg==WM_LBUTTONUP)loadimage(NULL,"rc\\start.jpg");
		}
	return 1;
}

bool ClickMSGSml(char title[],char str[],int type)
{
	getimage(&back2,0,0,640,520);	getimage(&backmo,0,0,640,520);
	Mosaic(&backmo, 15, 0, 0);		putimage(0, 0, &backmo);
	putimage(180,180,&messagesml);	getimage(&close,429,179,24,24);

	if(type==1)	{
		putimage(195,220,50,50,&icons,110,0); outtextxy(410, 280, _T("确定"));
		mciSendString("play error from 0", NULL, 0, NULL);	}
	else if(type==2)	{
		putimage(195,220,50,50,&icons,110,51);  outtextxy(410, 280, _T("确定"));
		mciSendString("play info from 0", NULL, 0, NULL);	}
	else if(type==3){
		putimage(195,220,50,50,&icons,160,0);  outtextxy(360, 280, _T("是(Y)"));outtextxy(410, 280, _T("否(N)"));
		mciSendString("play info from 0", NULL, 0, NULL);	}
	
	RECT r={260,220,435,334};
	outtextxy(185, 185,title);
	drawtext(str, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
	
	while(true)
	{
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())	{
				case 13:if(type==3){mciSendString("play error from 0", NULL, 0, NULL);break;}
				case 27:putimage(0,0,&back2);return 0;
				case 'Y': case 'y': if(type==3){putimage(0,0,&back2);return 1;}
				case 'N': case 'n': if(type==3){putimage(0,0,&back2);return 0;}
				default:mciSendString("play error from 0", NULL, 0, NULL);break;}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();
			if(m.x>430 && m.x<452 && m.y>180 && m.y<202 || m.x>405 && m.x<450 && m.y>275 && m.y<300)
			{
				solidrectangle(-1,499,640,520);outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				if(m.x>430 && m.x<452 && m.y>180 && m.y<202)rectangle(430,180,452,202);
				else rectangle(405,275,450,300);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);
					putimage(0,0,&back2);return 0;}
			}
			else if(type==3 && m.x>355 && m.x<400 && m.y>275 && m.y<300)
			{
				solidrectangle(-1,499,640,520);outtextxy(5, 502, _T("提示：点击继续程序。"));
				rectangle(355,275,400,300);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);
					putimage(0,0,&back2);return 1;}
			}
			else
			{
				solidrectangle(-1,499,640,520);outtextxy(5, 502, _T("提示：点击×或“确定”关闭，或者在键盘按回车或Esc。"));
				setlinecolor(WHITE);//恢复上一个鼠标位置的边框
				putimage(429,179,&close);	rectangle(405,275,450,300);rectangle(355,275,400,300);
				setlinecolor(RGB(45,180,160));
			}
		}
	}
}

bool ClickMSG()
{
	getimage(&close,527,99,24,24);
	while(true)
	{
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())
			{
				case 13://getch()中13为回车
				case 27:return 1;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;
			}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();
			if(m.x>528 && m.x<550 && m.y>100 && m.y<122){
				solidrectangle(-1,499,640,520); outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				rectangle(528,101,548,121);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);return 1;}}
			else
			{
				solidrectangle(-1,499,640,520);
				outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));
				putimage(527,99,&close);
			}
		}
	}
}

int ClickMSGSet()
{
	getimage(&backmo,0,0,640,520);
	Mosaic(&backmo, 15, 0, 0);
	putimage(0, 0, &backmo);

	putimage(100,100,&message);
	getimage(&close,527,99,24,24);

	outtextxy(104, 104, _T("设置"));
	outtextxy(150, 150, _T("修改昵称(C)"));	outtextxy(150, 200, _T("修改密码(P)"));
	outtextxy(150, 250, _T("删除账户(D)"));	outtextxy(150, 300, _T("新建账户(N)"));
	setlinecolor(BLACK);					setlinestyle(PS_JOIN_MITER|PS_SOLID,1);
	line(270,140,270,330);
	setlinecolor(RGB(45,180,160));			setlinestyle(PS_JOIN_MITER|PS_SOLID,3);
	outtextxy(300, 150, _T("选择难度(选择后将重新开局)"));	outtextxy(300, 180, _T("简单(E)"));
	outtextxy(380, 180, _T("中等(M)"));		outtextxy(460, 180, _T("困难(H)"));
	outtextxy(300, 230, _T("改变棋子颜色"));

	solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));

	setlinecolor(MAGENTA);
	if(dif==1)rectangle(295,175,360,200);
	else if(dif==2)rectangle(375,175,440,200);
	else if(dif==3)rectangle(455,175,520,200);
	setlinecolor(RGB(45,180,160));

	while(true)
	{
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())
			{
				case  13:case  27:return 1;
				case 'C':case 'c':return 101;
				case 'P':case 'p':return 102;
				case 'D':case 'd':return 103;
				case 'N':case 'n':return 104;
				case 'E':case 'e':setlinecolor(WHITE);rectangle(375,175,440,200);rectangle(455,175,520,200);setlinecolor(RGB(45,180,160));return 105;
				case 'M':case 'm':setlinecolor(WHITE);rectangle(295,175,360,200);rectangle(455,175,520,200);setlinecolor(RGB(45,180,160));return 106;
				case 'H':case 'h':setlinecolor(WHITE);rectangle(295,175,360,200);rectangle(375,175,440,200);setlinecolor(RGB(45,180,160));return 107;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;
			}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();
			if(m.x>528 && m.x<550 && m.y>100 && m.y<122){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				rectangle(528,101,548,121);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 1;}}
			else if(m.x>145 && m.x<250 && m.y>145 && m.y<170){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击修改玩家昵称，或者在键盘按“C”。"));
				rectangle(145,145,250,170);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 101;}}
			else if(m.x>145 && m.x<250 && m.y>195 && m.y<220){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击修改玩家密码，或者在键盘按“P”。"));
				rectangle(145,195,250,220);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 102;}}
			else if(m.x>145 && m.x<250 && m.y>245 && m.y<270){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击删除玩家账号，或者在键盘按“D”。"));
				rectangle(145,245,250,270);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 103;}}
			else if(m.x>145 && m.x<250 && m.y>295 && m.y<320){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击新建玩家账号，或者在键盘按“N”。"));
				rectangle(145,295,250,320);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 104;}}
			else if(m.x>295 && m.x<360 && m.y>175 && m.y<200 && dif!=1){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击设置游戏难度为简单，每放一个棋子加一分，或者在键盘按“E”。"));
				rectangle(295,175,360,200);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);
					setlinecolor(WHITE); 
					rectangle(375,175,440,200);rectangle(455,175,520,200);
					setlinecolor(RGB(45,180,160));return 105;}}
			else if(m.x>375 && m.x<440 && m.y>175 && m.y<200 && dif!=2){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击设置游戏难度为中等，每放一个棋子加三分，或者在键盘按“M”。"));
				rectangle(375,175,440,200);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);
					setlinecolor(WHITE);
					rectangle(295,175,360,200);rectangle(455,175,520,200);
					setlinecolor(RGB(45,180,160));return 106;}}
			else if(m.x>455 && m.x<520 && m.y>175 && m.y<200 && dif!=3){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击设置游戏难度为困难，每放一个棋子加五分，或者在键盘按“H”。"));
				rectangle(455,175,520,200);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	
					setlinecolor(WHITE); 
					rectangle(295,175,360,200);rectangle(375,175,440,200);
					setlinecolor(RGB(45,180,160));return 107;}}
			else if(m.x>295 && m.x<400 && m.y>225 && m.y<250){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击设置棋子颜色。"));
				rectangle(295,225,400,250);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 108;}}
			else
			{
				solidrectangle(-1,499,640,520);
				outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));
				
				setlinecolor(WHITE);//恢复上一个鼠标位置的边框
				putimage(527,99,&close);		rectangle(295,225,400,250);
				rectangle(145,145,250,170);		rectangle(145,195,250,220);
				rectangle(145,245,250,270);		rectangle(145,295,250,320);	
				if(dif!=1)rectangle(295,175,360,200);
				if(dif!=2)rectangle(375,175,440,200);
				if(dif!=3)rectangle(455,175,520,200);
				setlinecolor(RGB(45,180,160));
			}
		}
	}
}

int ClickToolFlash(int type,int time)
{
	int start=clock(),end=clock(); MOUSEMSG m2;
	while(end-start<time){
		if(MouseHit()){	m2= GetMouseMsg();
			if(m2.x<145 || m2.x>250 || m2.y<115+type*30 || m2.y>140+type*30)return 0;
			if(m2.uMsg==WM_LBUTTONUP)return 110;}
		end=clock();}
	return 1;
}

int ClickMSGTool()
{
	getimage(&backmo,0,0,640,520);
	Mosaic(&backmo, 15, 0, 0);
	putimage(0, 0, &backmo);

	putimage(100,100,&message);
	getimage(&close,527,99,24,24);

	outtextxy(104, 104, _T("商店"));
	outtextxy(150, 150, _T("灰棋光光(G)"));	outtextxy(150, 180, _T("速速解冻(R)"));
	outtextxy(150, 210, _T("我选棋子(A)"));	outtextxy(150, 240, _T("只来一块(O)"));
	outtextxy(150, 270, _T("可爱橡皮(E)"));	outtextxy(150, 300, _T("强力橡皮(S)"));
	setlinecolor(BLACK);					setlinestyle(PS_JOIN_MITER|PS_SOLID,1);
	line(270,140,270,330);
	setlinecolor(RGB(45,180,160));			setlinestyle(PS_JOIN_MITER|PS_SOLID,3);

	solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击右上角x关闭，或者在键盘按回车或Esc。"));
	
	while(true)
	{
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())
			{
				case  13:case  27:return 1;
				case 'G':case 'g':return 111;
				case 'R':case 'r':return 112;
				case 'A':case 'a':return 113;
				case 'O':case 'o':return 114;
				case 'E':case 'e':return 115;
				case 'S':case 's':return 116;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;
			}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();int Tool;
			if(m.x>528 && m.x<550 && m.y>100 && m.y<122){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				rectangle(528,101,548,121);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 1;}}
			else if(m.x>145 && m.x<250 && m.y>145 && m.y<170){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买消除全部灰色棋子的道具，或者在键盘按“G”。"));
				rectangle(145,145,250,170);
				char help[]={"购买此道具后屏幕上所有灰色棋子将被消除，购买本工具将会消耗500个金币。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				while(1)
				{
					putimage(315,220,170,170,&tool,0,0);	Tool=ClickToolFlash(1,1000);
					if(Tool==110)return 111;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,170,0);	settextcolor(WHITE);
					outtextxy(372,320,"灰棋光光！");		settextcolor(BLACK);
					Tool=ClickToolFlash(1,1000);
					if(Tool==110)return 111;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,170,170);	Tool=ClickToolFlash(1,2000);
					if(Tool==110)return 111;				else if(Tool==0)break;
				}
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 111;}}
			else if(m.x>145 && m.x<250 && m.y>175 && m.y<200){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买解除灰色棋子的道具，或者在键盘按“R”。"));
				rectangle(145,175,250,200);
				char help[]={"购买此道具后屏幕上所有灰色棋子将可以被消除，购买本工具将会消耗200个金币。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				while(1)
				{
					putimage(315,220,170,170,&tool,0,0);	Tool=ClickToolFlash(2,1000);
					if(Tool==110)return 112;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,170,0);	settextcolor(WHITE);
					outtextxy(372,320,"速速解冻！");		settextcolor(BLACK);
					Tool=ClickToolFlash(4,1000);
					if(Tool==110)return 112;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,0,170);Tool=ClickToolFlash(2,2000);
					if(Tool==110)return 112;				else if(Tool==0)break;
				}
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 112;}}
			else if(m.x>145 && m.x<250 && m.y>205 && m.y<230){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买任选棋子样式的道具，或者在键盘按“A”。"));
				rectangle(145,205,250,230);
				char help[]={"购买此道具后您可以通过鼠标滚轮任意选择一次当前棋子的样式，购买本工具将会消耗50个金币。悔棋后不能恢复道具。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				putimage(315,210,&toolseven);
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 113;}}
			else if(m.x>145 && m.x<250 && m.y>235 && m.y<260){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买只占一格的棋子，或者在键盘按“O”。"));
				rectangle(145,235,250,260);
				char help[]={"购买此道具后当前棋子将变为单位棋子一次，该棋子可以放在任意空位置，购买本工具将会扣除50分。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				while(1)
				{
					putimage(315,220,170,170,&tool,0,0);	Tool=ClickToolFlash(4,1000);
					if(Tool==110)return 114;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,0,340);	Tool=ClickToolFlash(4,1000);
					if(Tool==110)return 114;				else if(Tool==0)break;
					putimage(315,220,170,170,&tool,170,340);Tool=ClickToolFlash(4,2000);
					if(Tool==110)return 114;				else if(Tool==0)break;
				}
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 114;}}
			else if(m.x>145 && m.x<250 && m.y>265 && m.y<290){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买一块可擦除单位棋子的橡皮，或者在键盘按“O”。"));
				rectangle(145,265,250,290);
				char help[]={"购买此道具后您可以擦除不是灰色的单位棋子一次，购买本工具将会扣除50分。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				while(1)
				{
					putimage(315,220,170,170,&tool,0,0);	Tool=ClickToolFlash(5,1000);
					if(Tool==110)return 115;				else if(Tool==0)break;
					putimage(315,218,170,170,&tool,0,510);	Tool=ClickToolFlash(5,1000);
					if(Tool==110)return 115;				else if(Tool==0)break;
					putimage(315,218,170,170,&tool,170,510);Tool=ClickToolFlash(5,2000);
					if(Tool==110)return 115;				else if(Tool==0)break;
				}
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 115;}}
			else if(m.x>145 && m.x<250 && m.y>295 && m.y<320){
				setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				solidrectangle(-1,499,640,520);		outtextxy(5, 502, _T("提示：点击购买一块可擦除单位棋子的橡皮，或者在键盘按“O”。"));
				rectangle(145,295,250,320);
				char help[]={"购买此道具后您可以擦除任意3x3面积内的棋子一次，购买本工具将会扣除100分。"};
				RECT r = {300,140,530,350}; 		drawtext(help, &r, DT_WORDBREAK | DT_WORD_ELLIPSIS );
				while(1)
				{
					putimage(315,220,170,170,&tool,0,0);	Tool=ClickToolFlash(6,1000);
					if(Tool==110)return 116;				else if(Tool==0)break;
					putimage(315,218,170,170,&tool,0,680);	Tool=ClickToolFlash(6,1000);
					if(Tool==110)return 116;				else if(Tool==0)break;
					putimage(315,218,170,170,&tool,170,680);Tool=ClickToolFlash(6,2000);
					if(Tool==110)return 116;				else if(Tool==0)break;
				}
				if(m.uMsg==WM_LBUTTONUP){mciSendString("play click from 0", NULL, 0, NULL);	return 116;}}
			else
			{
				solidrectangle(-1,499,640,520);
				outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));
				
				setlinecolor(WHITE);//恢复上一个鼠标位置的边框
				putimage(527,99,&close);			setfillcolor(WHITE);
				solidrectangle(300,140,530,390);	setfillcolor(RGB(45, 180, 160));
				rectangle(145,145,250,170);			rectangle(145,175,250,200);
				rectangle(145,205,250,230);			rectangle(145,235,250,260);	
				rectangle(145,265,250,290);			rectangle(145,295,250,320);
				setlinecolor(RGB(45,180,160));
			}
		}
	}

	
	return 0;
}

int ClickMSGRGB()
{
	putimage(100,100,&message);
	putimage(140,140,&colorimg);
	outtextxy(104, 104, _T("设置颜色"));
	
	int color;
	while(true)
	{
		if(kbhit())
		{
			mciSendString("play clickkb from 0", NULL, 0, NULL);
			switch(getch())
			{
				case 13://getch()中13为回车
				case 27:return 1;
				default:mciSendString("play error from 0", NULL, 0, NULL);break;
			}
		}
		else if(MouseHit())
		{
			m = GetMouseMsg();
			if(m.x>528 && m.x<550 && m.y>100 && m.y<122){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击关闭对话框。"));
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	return 1;}}
			else if(m.x>=140 && m.x<=380 && m.y>=140 && m.y<=380){
				solidrectangle(-1,499,640,520);	outtextxy(5, 502, _T("提示：点击选择颜色(看我的颜色多炫~~)。"));
				color=HSLtoRGB((float)(m.x-140)*1.5,((float)(380-m.y))/240,(float)0.5);
				setlinecolor(color);	setfillcolor(color);
				solidrectangle(400,140,450,190);
				if(m.uMsg==WM_LBUTTONUP){
					mciSendString("play click from 0", NULL, 0, NULL);	
					setlinecolor(RGB(45,180,160));	setfillcolor(RGB(45,180,160));
					return color;}}
			else
			{
				setlinecolor(RGB(45,180,160));	setfillcolor(RGB(45,180,160));solidrectangle(-1,499,640,520);
				outtextxy(5, 502, _T("提示：点击右上角×关闭，或者在键盘按回车或Esc。"));
			}
		}
	}
}
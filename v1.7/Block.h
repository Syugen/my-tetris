#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

void BlockColor(IMAGE *pimg, int RGBC)
{
	int	width	= pimg->getwidth();		// 图像的宽
	int	height	= pimg->getheight();	// 图像的高
	DWORD* pMem = GetImageBuffer(pimg);
	float H,S,L;	RGBtoHSL(RGBC_,&H,&S,&L);
	L=L+(float)0.3;
	for (i=0;i<height;i++)
	for (j=0;j<width;j++)//太坑爹了！pMem[i*width+j]中RGB存放顺序是BGR
	{
		if(pMem[i*width+j]==15539236||pMem[i*width+j]==BGR(RGBC_)||pMem[i*width+j]==BGR(HSLtoRGB(H,S,L)))
			pMem[i*width+j]=BGR(RGBC);
	}
}

void BlockTemp()
{
	left=6+54*(co%10-1),top=84+52*(co/10-1);
	float H,S,L;	RGBtoHSL(RGBC,&H,&S,&L);
	L=L+(float)0.3;
	BlockColor(&blockredtemp, HSLtoRGB(H,S,L));
	if(NewBlock==0 && (TypeBlock==0 || TypeBlock==2))//画临时图
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co%10<=6)putimage(left-1+108,top-1,&blockredtemp);	if(co%10<=5)putimage(left-1+162,top-1,&blockredtemp);
	}
	else if(NewBlock==0 && (TypeBlock==1 || TypeBlock==3))
	{
		putimage(left-1,top-1,&blockredtemp);					if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);
		if(co/10<=6)putimage(left-1,top-1+104,&blockredtemp);	if(co/10<=5)putimage(left-1,top-1+156,&blockredtemp);
	}
	else if(NewBlock==1 && TypeBlock==0)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co/10>=3)putimage(left-1,top-1-104,&blockredtemp);
	}
	else if(NewBlock==1 && TypeBlock==1)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co%10<=6)putimage(left-1+108,top-1,&blockredtemp);
	}
	else if(NewBlock==1 && TypeBlock==2)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co/10<=6)putimage(left-1,top-1+104,&blockredtemp);
	}
	else if(NewBlock==1 && TypeBlock==3)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co%10>=3)putimage(left-1-108,top-1,&blockredtemp);
	}
	else if(NewBlock==2 && TypeBlock==0)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co/10>=3)putimage(left-1,top-1-104,&blockredtemp);
	}
	else if(NewBlock==2 && TypeBlock==1)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co%10<=6)putimage(left-1+108,top-1,&blockredtemp);
	}
	else if(NewBlock==2 && TypeBlock==2)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co/10<=6)putimage(left-1,top-1+104,&blockredtemp);
	}
	else if(NewBlock==2 && TypeBlock==3)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co%10>=3)putimage(left-1-108,top-1,&blockredtemp);
	}
	else if(NewBlock==3 && TypeBlock==0)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);	if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);
	}
	else if(NewBlock==3 && TypeBlock==1)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);
	}
	else if(NewBlock==3 && TypeBlock==2)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);	if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);
	}
	else if(NewBlock==3 && TypeBlock==3)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);
		if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);	if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);
	}
	else if(NewBlock==4 && (TypeBlock==0 || TypeBlock==2))
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10>=2)putimage(left-1-54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co%10<=7 && co/10>=2)putimage(left-1+54,top-1-52,&blockredtemp);
	}
	else if(NewBlock==4 && (TypeBlock==1 || TypeBlock==3))
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co%10<=7 && co/10<=7)putimage(left-1+54,top-1+52,&blockredtemp);
	}
	else if(NewBlock==5 && (TypeBlock==0 || TypeBlock==2))
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10>=2)putimage(left-1,top-1-52,&blockredtemp);	if(co%10>=2 && co/10>=2)putimage(left-1-54,top-1-52,&blockredtemp);
	}
	else if(NewBlock==5 && (TypeBlock==1 || TypeBlock==3))
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co%10<=7 && co/10>=2)putimage(left-1+54,top-1-52,&blockredtemp);
	}
	else if(NewBlock==6)
	{
		putimage(left-1,top-1,&blockredtemp);					if(co%10<=7)putimage(left-1+54,top-1,&blockredtemp);
		if(co/10<=7)putimage(left-1,top-1+52,&blockredtemp);	if(co%10<=7 && co/10<=7)putimage(left-1+54,top-1+52,&blockredtemp);
	}
	else if(NewBlock==7)	putimage(left-1,top-1,&blockredtemp);
	else if(NewBlock==8)	putimage(left-1,top-1,&block);
	else if(NewBlock==9)//消除3x3棋子
	{
		putimage(left-1,top-1,&block);
		if(co/10>=2 && co%10>=2)putimage(left-1-54,top-1-52,&block);
		if(co/10>=2)			putimage(left-1,top-1-52,&block);
		if(co/10>=2 && co%10<=7)putimage(left-1+54,top-1-52,&block);
		if(co%10>=2)			putimage(left-1-54,top-1,&block);
		if(co%10<=7)			putimage(left-1+54,top-1,&block);
		if(co/10<=7 && co%10>=2)putimage(left-1-54,top-1+52,&block);
		if(co/10<=7)			putimage(left-1,top-1+52,&block);
		if(co/10<=7 && co%10<=7)putimage(left-1+54,top-1+52,&block);
	}

	fillrectangle(-1,499,640,520);//输出提示
	char str[100];
	sprintf(str, "提示：目前位于第%d行第%d列，鼠标左键确认，右键旋转；键盘方向键移动，空格键旋转，回车键确认。", co/10,co%10);
	settextstyle(16, 0, _T("宋体"));
	outtextxy(5, 502, str);

}

bool BlockDraw(int NewNextB,int co)//返回0不能放置，1成功
{//加入棋子
	if(!co){
		putimage(432,288,208,212,&map,432,288);
		outtextxy(470, 300, _T("下个棋子:"));}
	
	left=6+54*(co%10-1),top=84+52*(co/10-1);
	if(NewNextB==0 && (!co || (TypeBlock==0 || TypeBlock==2) && co%10<=5 && Has[co]==0 && Has[co+1]==0 && Has[co+2]==0 && Has[co+3]==0))
	{
		if(!co){left=430;top=374;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1+108,top-1,&blockred);		putimage(left-1+162,top-1,&blockred);
		if(co){Has[co]=Has[co+1]=Has[co+2]=Has[co+3]=1;}
	}
	else if(NewNextB==0 && (TypeBlock==1 || TypeBlock==3) && co/10<=5 && Has[co]==0 && Has[co+10]==0 && Has[co+20]==0 && Has[co+30]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1,top-1+52,&blockred);
		putimage(left-1,top-1+104,&blockred);		putimage(left-1,top-1+156,&blockred);
		Has[co]=Has[co+10]=Has[co+20]=Has[co+30]=1;
	}

	//第2种 L型
	else if(NewNextB==1 && (!co || TypeBlock==0 && co%10<=7 && co/10>=3 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co-20]==0))
	{
		if(!co){left=480;top=425;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1,top-1-104,&blockred);
		if(co){Has[co]=Has[co+1]=Has[co-10]=Has[co-20]=1;}
	}
	else if(NewNextB==1 && TypeBlock==1 && co%10<=6 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+2]==0 && Has[co+10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1+108,top-1,&blockred);
		Has[co]=Has[co+1]=Has[co+2]=Has[co+10]=1;
	}
	else if(NewNextB==1 && TypeBlock==2 && co%10>=2 && co/10<=6 && Has[co]==0 && Has[co-1]==0 && Has[co+10]==0 && Has[co+20]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1-54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1,top-1+104,&blockred);
		Has[co]=Has[co-1]=Has[co+10]=Has[co+20]=1;
	}
	else if(NewNextB==1 && TypeBlock==3 && co%10>=3 && co/10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co-2]==0 && Has[co-10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1-54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1-108,top-1,&blockred);
		Has[co]=Has[co-1]=Has[co-10]=Has[co-2]=1;
	}

	//第3种 反L型
	else if(NewNextB==2 && (!co || TypeBlock==0 && co%10>=2 && co/10>=3 && Has[co]==0 && Has[co-1]==0 && Has[co-10]==0 && Has[co-20]==0))
	{
		if(!co){left=534;top=425;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1-54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1,top-1-104,&blockred);
		if(co){Has[co]=Has[co-1]=Has[co-10]=Has[co-20]=1;}
	}
	else if(NewNextB==2 && TypeBlock==1 && co%10<=6 && co/10>=2 && Has[co]==0 && Has[co-10]==0 && Has[co+1]==0 && Has[co+2]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1+108,top-1,&blockred);
		Has[co]=Has[co-10]=Has[co+1]=Has[co+2]=1;
	}
	else if(NewNextB==2 && TypeBlock==2 && co%10<=7 && co/10<=6 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co+20]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1,top-1+104,&blockred);
		Has[co]=Has[co+1]=Has[co+10]=Has[co+20]=1;
	}
	else if(NewNextB==2 && TypeBlock==3 && co%10>=3 && co/10<=7 && Has[co]==0 && Has[co-1]==0 && Has[co-2]==0 && Has[co+10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1-54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1-108,top-1,&blockred);
		Has[co]=Has[co-1]=Has[co-2]=Has[co+10]=1;
	}

	//第4种 ⊥型
	else if(NewNextB==3 && (!co || TypeBlock==0 && co/10>=2 && co%10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+1]==0 && Has[co-10]==0))
	{
		if(!co){left=507;top=400;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1-54,top-1,&blockred);		putimage(left-1,top-1-52,&blockred);
		if(co){Has[co]=Has[co-1]=Has[co+1]=Has[co-10]=1;}
	}
	else if(NewNextB==3 && TypeBlock==1 && co/10>=2 && co/10<=7 && co%10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co+10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1,top-1-52,&blockred);
		Has[co]=Has[co+1]=Has[co-10]=Has[co+10]=1;
	}
	else if(NewNextB==3 && TypeBlock==2 && co/10<=7 && co%10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+1]==0 && Has[co+10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1-54,top-1,&blockred);		putimage(left-1,top-1+52,&blockred);
		Has[co]=Has[co-1]=Has[co+1]=Has[co+10]=1;
	}
	else if(NewNextB==3 && TypeBlock==3 && co/10>=2 && co/10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+10]==0 && Has[co-10]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1,top-1+52,&blockred);
		putimage(left-1-54,top-1,&blockred);		putimage(left-1,top-1-52,&blockred);
		Has[co]=Has[co-1]=Has[co+10]=Has[co-10]=1;
	}

	//第5种 右上楼梯型
	else if(NewNextB==4 && (!co || (TypeBlock==0 || TypeBlock==2) && co%10<=7 && co%10>=2 && co/10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co-10]==0 && Has[co-9]==0))
	{
		if(!co){left=507;top=400;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1-54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1+54,top-1-52,&blockred);
		if(co){Has[co]=Has[co-1]=Has[co-10]=Has[co-9]=1;}
	}
	else if(NewNextB==4 && (TypeBlock==1 || TypeBlock==3) && co%10<=7 && co/10>=2 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co+11]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1+54,top-1+52,&blockred);
		Has[co]=Has[co+1]=Has[co-10]=Has[co+11]=1;
	}

	//第6种 左上楼梯型
	else if(NewNextB==5 && (!co || (TypeBlock==0 || TypeBlock==2) && co%10<=7 && co%10>=2 && co/10>=2 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co-11]==0))
	{
		if(!co){left=507;top=400;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1-52,&blockred);		putimage(left-1-54,top-1-52,&blockred);
		if(co){Has[co]=Has[co+1]=Has[co-10]=Has[co-11]=1;}
	}
	else if(NewNextB==5 && (TypeBlock==1 || TypeBlock==3) && co%10<=7 && co/10>=2 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co-9]==0)
	{
		putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1+54,top-1-52,&blockred);
		Has[co]=Has[co+1]=Has[co+10]=Has[co-9]=1;
	}

	//第7种 四方块型
	else if(NewNextB==6 && (!co || co/10<=7 && co%10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co+11]==0))
	{
		if(!co){left=480;top=350;putimage(left-1,top-1,&blockblue);}
		else putimage(left-1,top-1,&blockred);	putimage(left-1+54,top-1,&blockred);
		putimage(left-1,top-1+52,&blockred);		putimage(left-1+54,top-1+52,&blockred);
		if(co){Has[co]=Has[co+1]=Has[co+10]=Has[co+11]=1;}
	}
	else if(NewNextB==7)//单位棋子
	{
		putimage(left-1,top-1,&blockred);
		Has[co]=1;
	}
	else if(NewNextB==8 && Grey[co]==0)//消除单位棋子
	{
		putimage(left-1,top-1,&block);
		Has[co]=0;
	}
	else if(NewNextB==9)//消除3x3棋子
	{
		putimage(left-1,top-1,&block);Grey[co]=Has[co]=0;
		if(co/10>=2 && co%10>=2){putimage(left-1-54,top-1-52,&block);Grey[co-11]=Has[co-11]=0;}
		if(co/10>=2)			{putimage(left-1,top-1-52,&block);Grey[co-10]=Has[co-10]=0;}
		if(co/10>=2 && co%10<=7){putimage(left-1+54,top-1-52,&block);Grey[co-9]=Has[co-9]=0;}
		if(co%10>=2)			{putimage(left-1-54,top-1,&block);Grey[co-1]=Has[co-1]=0;}
		if(co%10<=7)			{putimage(left-1+54,top-1,&block);Grey[co+1]=Has[co+1]=0;}
		if(co/10<=7 && co%10>=2){putimage(left-1-54,top-1+52,&block);Grey[co+9]=Has[co+9]=0;}
		if(co/10<=7)			{putimage(left-1,top-1+52,&block);Grey[co+10]=Has[co+10]=0;}
		if(co/10<=7 && co%10<=7){putimage(left-1+54,top-1+52,&block);Grey[co+11]=Has[co+11]=0;}		
	}
	else 
	{
		putimage(6,84,&backsml);
		fillrectangle(-1,499,640,520);
		outtextxy(5, 502, "提示：不能放在这里！");
		mciSendString("play clickkberror from 0", NULL, 0, NULL);
		return 0;
	}
	getimage(&backsml,6,84,430,494);
	return 1;
}

void BlockDel()
{
	for(int ct=1;ct<=36;ct++)//从8x8开始往下降
	{
		int m,n;
		if	   (ct== 1 || ct== 2 || ct== 5 || ct== 9 || ct==14 || ct==21)m=8;
		else if(ct== 3 || ct== 4 || ct== 7 || ct==12 || ct==18 || ct==25)m=7;
		else if(ct== 6 || ct== 8 || ct==11 || ct==16 || ct==23 || ct==28)m=6;
		else if(ct==10 || ct==13 || ct==17 || ct==20 || ct==27 || ct==32)m=5;
		else if(ct==15 || ct==19 || ct==24 || ct==29 || ct==31 || ct==34)m=4;
		else if(ct==22 || ct==26 || ct==30 || ct==33 || ct==35 || ct==36)m=3;
		if	   (ct== 1 || ct== 3 || ct== 6 || ct==10 || ct==15 || ct==22)n=8;
		else if(ct== 2 || ct== 4 || ct== 8 || ct==13 || ct==19 || ct==26)n=7;
		else if(ct== 5 || ct== 7 || ct==11 || ct==17 || ct==24 || ct==30)n=6;
		else if(ct== 9 || ct==12 || ct==16 || ct==20 || ct==29 || ct==33)n=5;
		else if(ct==14 || ct==18 || ct==23 || ct==27 || ct==31 || ct==35)n=4;
		else if(ct==21 || ct==25 || ct==28 || ct==32 || ct==34 || ct==36)n=3;
		for(i=1;i<=9-m;i++)//定位消除左上角格子
		for(j=1;j<=9-n;j++)
		{
			int s,t;
			for(s=i;s<i+m;s++)//判断在该m阶方阵是否全有
			for(t=j;t<j+n;t++)
			{
				co=10*s+t;
				if(!Has[co])goto cant;
			}
			s--;t--;
			if(s==(i+m-1) && t==(j+n-1))
			{
				mciSendString("play score from 0", NULL, 0, NULL);
				for(s=i;s<i+m;s++)
				for(t=j;t<j+n;t++)
				{
					co=10*s+t;left=6+54*(t-1);right=52+54*(t-1);top=84+52*(s-1);bottom=130+52*(s-1);
					settextstyle(20, 0, _T("宋体"));settextcolor(WHITE);
					int deltay=15;
					while(--deltay){
						fillrectangle(left,top,right,bottom);
						outtextxy(left+15,top+deltay,"+1");Sleep(8);}
					if(!Grey[co]){
						putimage(left-1,top-1,&block);	Has[co]=0; }
					else putimage(left-1,top-1,&blockgrey);
				}
				settextstyle(16, 0, _T("宋体"));settextcolor(BLACK);
				score+= m*n;
			}
cant:;	}
	}
}

int BlockCanPut()
{
	for(TypeBlock=0;TypeBlock<=3;TypeBlock++)//四种样式
		for(i=1;i<=8;i++)
			for(j=1;j<=8;j++)
			{
				co=10*i+j;
				if(NewBlock==0 && (TypeBlock==0 || TypeBlock==2) && co%10<=5 && Has[co]==0 && Has[co+1]==0 && Has[co+2]==0 && Has[co+3]==0)return i*10+j;
				else if(NewBlock==0 && (TypeBlock==1 || TypeBlock==3) && co/10<=5 && Has[co]==0 && Has[co+10]==0 && Has[co+20]==0 && Has[co+30]==0)return i*10+j;
				else if(NewBlock==1 && TypeBlock==0 && co%10<=7 && co/10>=3 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co-20]==0)return i*10+j;
				else if(NewBlock==1 && TypeBlock==1 && co%10<=6 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+2]==0 && Has[co+10]==0)return i*10+j;
				else if(NewBlock==1 && TypeBlock==2 && co%10>=2 && co/10<=6 && Has[co]==0 && Has[co-1]==0 && Has[co+10]==0 && Has[co+20]==0)return i*10+j;
				else if(NewBlock==1 && TypeBlock==3 && co%10>=3 && co/10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co-2]==0 && Has[co-10]==0)return i*10+j;
				else if(NewBlock==2 && TypeBlock==0 && co%10>=2 && co/10>=3 && Has[co]==0 && Has[co-1]==0 && Has[co-10]==0 && Has[co-20]==0)return i*10+j;
				else if(NewBlock==2 && TypeBlock==1 && co%10<=6 && co/10>=2 && Has[co]==0 && Has[co-10]==0 && Has[co+1]==0 && Has[co+2]==0)return i*10+j;
				else if(NewBlock==2 && TypeBlock==2 && co%10<=7 && co/10<=6 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co+20]==0)return i*10+j;
				else if(NewBlock==2 && TypeBlock==3 && co%10>=3 && co/10<=7 && Has[co]==0 && Has[co-1]==0 && Has[co-2]==0 && Has[co+10]==0)return i*10+j;
				else if(NewBlock==3 && TypeBlock==0 && co/10>=2 && co%10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+1]==0 && Has[co-10]==0)return i*10+j;
				else if(NewBlock==3 && TypeBlock==1 && co/10>=2 && co/10<=7 && co%10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co+10]==0)return i*10+j;
				else if(NewBlock==3 && TypeBlock==2 && co/10<=7 && co%10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+1]==0 && Has[co+10]==0)return i*10+j;
				else if(NewBlock==3 && TypeBlock==3 && co/10>=2 && co/10<=7 && co%10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co+10]==0 && Has[co-10]==0)return i*10+j;
				else if(NewBlock==4 && (TypeBlock==0 || TypeBlock==2) && co%10<=7 && co%10>=2 && co/10>=2 && Has[co]==0 && Has[co-1]==0 && Has[co-10]==0 && Has[co-9]==0)return i*10+j;
				else if(NewBlock==4 && (TypeBlock==1 || TypeBlock==3) && co%10<=7 && co/10>=2 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co+11]==0)return i*10+j;
				else if(NewBlock==5 && (TypeBlock==0 || TypeBlock==2) && co%10<=7 && co%10>=2 && co/10>=2 && Has[co]==0 && Has[co+1]==0 && Has[co-10]==0 && Has[co-11]==0)return i*10+j;
				else if(NewBlock==5 && (TypeBlock==1 || TypeBlock==3) && co%10<=7 && co/10>=2 && co/10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co-9]==0)return i*10+j;
				else if(NewBlock==6 && co/10<=7 && co%10<=7 && Has[co]==0 && Has[co+1]==0 && Has[co+10]==0 && Has[co+11]==0)return i*10+j;
				else if(NewBlock==7 && Has[co]==0 || NewBlock==8 || NewBlock==9)return i*10+j;
			}
	mciSendString("play dead from 0", NULL, 0, NULL);
	SaveScore(); 
	
	for(i=11;i<89;i++)Has[i]=Grey[i]=0;
	level=1;score=0;pieceleft=15;huiqi=0;

	if(ClickMSGSml("游戏结束", "囧，您失败了……\n是否重玩？\n（否将返回初始界面）",3))return 2;
	else return 3;
}
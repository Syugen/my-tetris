#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdlib.h>

//定义结构体
struct user
{
	char name[16];
	char pass[16];
	int score;
	user *next;
};

struct SAVETEMP
{
	bool Has[89];
	bool Grey[89];
	int null;//没有用，但是没有他的话一旦悔棋level就丢了
	int level;
	int score;
	int pieceleft;
	char player[15];
	int New;
	SAVETEMP *next;
};

bool SaveReadClose()
{
	FILE *fp=fopen("rc\\close.save", "r");
	char fileplayer[15]={0};
	bool read=0;
	fscanf(fp, "%s", fileplayer);
	if(fileplayer[0])//如果有记录
	{
		char str[50];
		sprintf(str, "是否读取%s的游戏记录？", fileplayer);
		if(ClickMSGSml("提示", str, 3))read=1;
		else strcpy(fileplayer, "\0");
		fclose(fp);
	}
	return read;
}

void SaveBack(bool Has[],bool Grey[],INFO *nf,int *NewBlock,char player[])//悔棋
{
	int i;
	SAVETEMP *H=NULL,*p=NULL,*p_=NULL,*file;
	FILE *fp=fopen("rc\\temp.save","r+");
	H=(SAVETEMP*)malloc(sizeof(SAVETEMP));
	H->next=NULL;

	p=H;   
	while(!feof(fp)) 
	{  
		file=(SAVETEMP*)malloc(sizeof(SAVETEMP));  
		fscanf(fp,"%s %d %d %d %d ",file->player,&(file->level),&(file->score),&(file->pieceleft),&(file->New));
		for(int i=11;i<89;i++)fscanf(fp, "%d %d ", &file->Has[i],&file->Grey[i]); 
		file->next=NULL;//生成新结点 
		p->next=file;//插到尾部 
		p_=p;
		p=file;//p后移 	
	}

	p=p_;
	for(i=11;i<89;i++){Has[i]=p->Has[i];Grey[i]=p->Grey[i];}
	(*nf).level=p->level;			(*nf).score=p->score-5;
	(*nf).pieceleft=p->pieceleft;	*NewBlock=p->New;
	strcpy(player,p->player);	
	fclose(fp);

	p=H->next;
	fp=fopen("rc\\temp.save","w+");
	for( ;p!=p_->next;p=p->next)
	{
		fprintf(fp,"\n%s %d %d %d %d ",p->player,p->level,p->score-5,p->pieceleft,p->New);
		for(i=11;i<89;i++)fprintf(fp, "%d %d ", p->Has[i],p->Grey[i]);
	}
	fclose(fp);

	SAVETEMP *del; 
	p=H;
	while(p)
	{
		del=p;
		p=p->next;
		free(del);
	}
}

void SaveRead(user *H)//用于读取list.save文件，因为最常用，其他读取未制作函数
{
	FILE *fp=fopen("rc\\list.save","r");
	user *p,*file;  
	p=H;
	fscanf(fp, "\n");
	while(!feof(fp)) 
	{  
		file=(user*)malloc(sizeof(user));
		fscanf(fp, "%s\t%s", file->name,file->pass);
		file->next=NULL;
		p->next=file;
		p=file;
	} 
	fclose(fp);
}

bool SaveInputPass(user *p,user *input)
{
	if(p->pass[0]=='0')return 1;//没有密码
	int time=6;//最多错5次
	while(strcmp(p->pass,input->pass) && --time)
	{
		char str[50];
		sprintf(str, "抱歉，输入错误，您还有%d次机会。", time);
		if(time<5)ClickMSGSml("提示",str,2);
		if(!ClickInput("方块拼图",input->pass,"请输入【密码】。按取消重新输入昵称。",1))return 0;
	}
	if(!time){ClickMSGSml("密码错误五次","抱歉，请重新输入昵称。",1);return 0;}
	return 1;//只有输入正确才会返回1
}

void SaveNew(user *input)
{
	FILE *fp=fopen("rc\\list.save","a+");
	if(!ClickInput("方块拼图",input->pass, "欢迎新用户，请设置【密码】。不设密码则输入0或不输入。",1))
		strcpy(input->pass,"0");//点取消没有设置密码
	if(input->pass[0]=='\0')strcpy(input->pass,"0");//没设密码
	fprintf(fp,"\n%s\t%s",input->name,input->pass);//存入信息
	input->score=0;
	fclose(fp);
}

void SaveNodeDel(user *H)
{
	user *del,*p=H; 
	while(p)
	{
		del=p;  //del保存当前的p
		p=p->next;
		free(del);
	}
}

bool SaveLogIn(char player[])
{
wrong://输入错误5次的起始点
	user *input=(user*)malloc(sizeof(user));
	
	if(!ClickInput("方块拼图",input->name, "请输入用户名。\n想看电脑自娱自乐请输入computer",0))
		return 0;
	if(!input->name[0])
	{
		ClickMSGSml("错误","昵称不能为空！",1);
		goto wrong;
	}

	//生成链表
	user *H=NULL,*p=NULL;
//	FILE *fp=fopen("rc\\list.save","r+");
	H=(user*)malloc(sizeof(user));
	H->next=NULL;
	SaveRead(H);

	p=H->next;
	for( ;p!=NULL;p=p->next)
	{
		if(!strcmp(p->name,input->name))//如果相同
		{
			if(!SaveInputPass(p,input))goto wrong;//1正常0回到输入昵称
			FILE *fp2=fopen("rc\\score.save","r");
			user *file=(user*)malloc(sizeof(user));  //读取老用户最高分当做输入值
			fscanf(fp2, "\n");  
			do{
				fscanf(fp2, "%s\t%d", file->name,&(file->score)); 
			}while(strcmp(file->name,input->name)&&!feof(fp2));//char ss[15];sprintf(ss,"%s %s",file->name,input->name);MessageBox(0,ss,"",MB_OK);
			if(strcmp(file->name,input->name))input->score=0;//没有成绩（建立账号后没有玩）
			else input->score=file->score;
			free(file);	fclose(fp2); //fclose(fp);
			break;
		}
		
	}
	if(!p) SaveNew(input);//新用户
	char str[50];
	sprintf(str, "您好,%s。\n您的最高分是%d分!\n游戏即将开始!", input->name,input->score);
	ClickMSGSml("您好",str,2);
	strcpy(player,input->name);
	SaveNodeDel(H);
	free(input);
	return 1;
}

bool SaveChange(int type,char player[])//type=1改名字2改密码
{
wrong:
	user *input=(user*)malloc(sizeof(user));
	if(!ClickInput("方块拼图",input->name,"请输入用户名。",0))
		return 0;
	if(!input->name[0] || !strcmp(input->name,"在这里输入…")){
		ClickMSGSml("错误","昵称不能为空！",1);goto wrong;}
	
	//生成链表，实际上是SaveRead的变体
	int find=0;	char oldname[15];
	user *H=NULL,*p,*file,*q;
	FILE *fp=fopen("rc\\list.save","r");
	H=(user*)malloc(sizeof(user));
	H->next=NULL;
	p=H;
	fscanf(fp, "\n");
	while(!feof(fp)) 
	{  
		file=(user*)malloc(sizeof(user));  
		fscanf(fp, "%s\t%s", file->name,file->pass);  
		file->next=NULL;//生成新结点 
		p->next=file;
		q=p;//q是p前一个
		p=file;//p后移 
	
		if(!strcmp(p->name,input->name))//如果相同
		{
			strcpy(oldname,p->name);
			if(!SaveInputPass(p,input))goto wrong;//1正常0回到输入昵称
			if(type==1)
			{
				find=1;
				if(!ClickInput("方块拼图",input->name,"请输入新用户名。",0))
					return 0;
				if(!input->name[0] || !strcmp(input->name,"在这里输入…")){
					ClickMSGSml("错误","昵称不能为空！",1);goto wrong;}

				strcpy(input->pass,p->pass);
				
				input->next=NULL;
				p=q;//p倒退
				p->next=input;
				p=input;
			}
			else if(type==2)
			{
				find=1;
				char secure[15]={"1"};
				while(strcmp(secure,input->pass))
				{
					if(!ClickInput("方块拼图",input->pass,"请输入新【密码】，不设置直接确定。",1))
						return 0;
					if(!strcmp(input->pass,"在这里输入…")||input->pass[0]=='\0')strcpy(input->pass,"0");
					if(!ClickInput("方块拼图",secure,"请【确认】新【密码】。",1))
						return 0;
					if(!strcmp(secure,"在这里输入…")||secure[0]=='\0' )strcpy(secure,"0");
					if(strcmp(secure,input->pass)) ClickMSGSml("错误","两次密码不一致，请重新输入！",1);
				}
				strcpy(input->name,p->name);
				
				input->next=NULL;
				p=q;//p倒退
				p->next=input;
				p=input;
			}
		}
	}
	fclose(fp);
	
	if(!find){ClickMSGSml("错误","该用户不存在，请更换！",1);goto wrong;}

	if(type==1)//改名时检查重名
	{
		user *H_=NULL;
		H_=(user*)malloc(sizeof(user));
		H_->next=NULL;
		SaveRead(H_);

		p=H_;
		for( ;p!=NULL;p=p->next)
		{
			if(!strcmp(p->name,input->name))//如果相同
			{
				fclose(fp);
				strcpy(input->pass,"0");
				ClickMSGSml("错误","该名称已经存在，请【重新开始】！",1);
				goto wrong;
			}
		}
		fclose(fp);

		//如果是当前用户要把当前player改掉
		if(!strcmp(player,oldname))	strcpy(player,input->name);
		//不重名的话把score里面的名字全改了
		fp=fopen("rc\\score.save","r");
		user *H2=NULL;
		H2=(user*)malloc(sizeof(user));
		H2->next=NULL;
		p=H2;
		fscanf(fp, "\n");
		while(!feof(fp)) 
		{  
			file=(user*)malloc(sizeof(user));  
			fscanf(fp, "%s\t%d", file->name,&file->score);  
			file->next=NULL;//生成新结点 
			p->next=file;//插到尾部 
			p=file;//p后移   
			if(!strcmp(p->name,oldname))strcpy(p->name,input->name);
		} 
		fclose(fp);

		p=H2->next;
		fp=fopen("rc\\score.save","w+");
		for( ;p!=NULL;p=p->next)
			fprintf(fp,"\n%s\t%d",p->name,p->score);
		fclose(fp);
		SaveNodeDel(H2);
	}

	p=H->next;
	fp=fopen("rc\\list.save","w+");
	for( ;p!=NULL;p=p->next)
		fprintf(fp,"\n%s\t%s",p->name,p->pass);
	fclose(fp);
	SaveNodeDel(H);//free(input);input已经free过了
	ClickMSGSml("成功","修改成功！",2);
	return 1;
}

bool SaveCreate()
{
wrong:
	user *input=(user*)malloc(sizeof(user));
	if(!ClickInput("方块拼图",input->name,"请输入您要新建的用户名。",0))
		return 0;
	if(!input->name[0] || !strcmp(input->name,"在这里输入…")){
		ClickMSGSml("错误","昵称不能为空！",1);goto wrong;}
	
	//生成链表
	user *H=NULL,*p=NULL;
//	FILE *fp=fopen("rc\\list.save","r");
	H=(user*)malloc(sizeof(user));
	H->next=NULL;
	SaveRead(H);

	p=H;
	for( ;p!=NULL;p=p->next)
	{
		if(!strcmp(p->name,input->name))//如果相同
		{
		//	fclose(fp);
			ClickMSGSml("错误","该名称已经存在，请更换！",1);
			goto wrong;
		}
	}
	SaveNew(input);//只有没找到才可能执行这步

	char str[50];
	sprintf(str, "您好,%s!\n您的最高分是%d分!", input->name,input->score);
	ClickMSGSml("您好",str,2);
	
	//删除链表
	SaveNodeDel(H);free(input);
	return 1;
}

bool SaveDelete(char player[])
{
wrong:
	user *input=(user*)malloc(sizeof(user));
	if(!ClickInput("方块拼图",input->name,"请输入您要删除的用户名。",0))
		return 0;
	if(!input->name[0]){
		ClickMSGSml("错误","昵称不能为空！",1);goto wrong;}
	
	//生成链表
	user *H=NULL,*p=NULL;
//	FILE *fp=fopen("rc\\list.save","r");
	H=(user*)malloc(sizeof(user));
	H->next=NULL;
	SaveRead(H);

	p=H;
	for( ;p!=NULL;p=p->next)
	{
		if(p->next && !strcmp(p->next->name,input->name))//如果相同
		{
			if(!SaveInputPass(p->next,input))goto wrong;//1正常0回到输入昵称
			if(!strcmp(player,input->name))ClickMSGSml("提示","您正在删除当前用户，本账号将在关闭后删除~",2);
			else ClickMSGSml("提示","该用户已经被删除！\n游戏记录将保存~",2);
			user *q=p->next;
			p->next=q->next;
			free(q);  
			break;
		}
	}
//	fclose(fp);
	if(!p){ClickMSGSml("错误","该用户不存在，请更换！",1);goto wrong;}
	
	p=H->next;
	FILE *fp=fopen("rc\\list.save","w+");
	for( ;p!=NULL;p=p->next)
		fprintf(fp,"\n%s\t%s",p->name,p->pass);
	fclose(fp);
	//删除链表
	SaveNodeDel(H);
	return 1;
}

void SaveScore(int score,char player[])
{
	//生成链表，实际上是SaveRead的变体
	int i=0;bool add=0;
	user *H=NULL,*p,*q,*file,*input;
	FILE *fp=fopen("rc\\score.save","r");
	H=(user*)malloc(sizeof(user));
	H->next=NULL;
	p=H;
	fscanf(fp, "\n");
	while(!feof(fp)) 
	{  
		file=(user*)malloc(sizeof(user));  
		fscanf(fp, "%s\t%d", file->name,&(file->score));
		file->next=NULL;//生成新结点 
		p->next=file;
		q=p;
		p=file;//p后移 

		if(!add && score>p->score)//如果相同且为最高分
		{
			input=(user*)malloc(sizeof(user));
			strcpy(input->name,player);
			strcpy(input->pass,p->pass);
			input->score=score;
			
			input->next=NULL;
			q->next=input;
			input->next=p;
			add=1;
		}
		i++;
	}
	fclose(fp);

	p=H->next;
	fp=fopen("rc\\score.save","w+");
	for( ;p!=NULL;p=p->next)
		fprintf(fp,"\n%s\t%d",p->name,p->score);
	fclose(fp);

	SaveNodeDel(H);
}
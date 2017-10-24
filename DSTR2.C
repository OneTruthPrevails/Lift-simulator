#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<graphics.h>
#include<time.h>
#define TRUE 1
#define FALSE 0
#define capacity 6
#define maxfloor 10

typedef struct node
{
	int tofloor,onfloor;
	struct node *prev,*next;
}node;

typedef struct lift
{
	node *start,*end;
	int count;
}lift;

void insert(lift *t,int tofloor,int onfloor)
{

	node *p,*q;
	p=(node*)malloc(sizeof(node));
	p->tofloor=tofloor;
	p->onfloor=onfloor;
	if(t->start==NULL)
	{
		p->next=p->prev=NULL;
		t->start=t->end=p;
		t->count++;
		return;
	}
	q=t->start;
	if(t->count==1&&tofloor>q->tofloor)
	{
		p->next=p->prev=NULL;
		p->prev=t->end;
		t->end->next=p;
		t->end=p;
		t->count++;
		return;
	}
	while(q->next!=NULL)
	{
		if(q->tofloor<=tofloor)
			q=q->next;
		else
			break;
	}
	if(q==t->start)
	{
		p->next=p->prev=NULL;
		p->next=t->start;
		t->start->prev=p;
		t->start=p;
		t->count++;
		return;
	}
	if(q==t->end)
	{
		p->next=p->prev=NULL;
		p->prev=t->end;
		t->end->next=p;
		t->end=p;
		t->count++;
		return;
	}
	q=q->prev;
	p->next=q->next;
	p->next->prev=p;
	q->next=p;
	p->prev=q;
	t->count++;
	return;
}

void delete1(lift *t,node *p)
{
	node *q;
	if(p==t->start)
	{
		if(t->start==t->end)
		{
			t->start=t->end=NULL;
			t->count--;
			free(p);
			return;
		}
		else
		{
			t->start=t->start->next;
			t->start->prev=NULL;
			t->count--;
			free(p);
			return;
		}
	}
	else if(p==t->end)
	{
		if(t->start==t->end)
		{
			t->start=t->end=NULL;
			t->count--;
			free(p);
			return;
		}
		else
		{
			t->end=t->end->prev;
			t->end->next=NULL;
			t->count--;
			free(p);
			return;
		}
	}
	else
	{
		q=p->prev;
		q->next=p->next;
		q->next->prev=q;
		t->count--;
		free(p);
		return;
	}
}

int isliftfull(lift *t)
{
	if(t->count==capacity-1)
		return TRUE;
	else
		return FALSE;
}

node* searchup(lift *t,int onfloor)
{
	node *q;
	q=t->start;
	while(q!=NULL)
	{
		if(q->tofloor>onfloor&&q->onfloor==onfloor)
			return q;
		q=q->next;
	}
	return NULL;
}

node* searchdown(lift *t,int onfloor)
{
	node *q;
	q=t->end;
	while(q!=NULL)
	{
		if(q->tofloor<onfloor&&q->onfloor==onfloor)
			return q;
		q=q->prev;
	}
	return NULL;
}

void printtime(FILE *record)
{
	time_t mytime;
	mytime=time(0);
	fprintf(record,"Last used on: %s\n",ctime(&mytime));
	return;
}

void displaylift(int floor)
{
	int gd=DETECT,gm,i;
	clrscr();
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	setfillstyle(XHATCH_FILL,CYAN);
	floodfill(0,0,BLUE);
	rectangle(129,29,529,449);
	rectangle(129,29,179,449);
	rectangle(479,29,529,449);
	for(i=0;i<=150;i++)
	{
		setcolor(CYAN);
		rectangle(179,29,329-i,449);
		rectangle(329+i,29,479,449);
		delay(20);
		setcolor(BLACK);
		rectangle(179,29,329-i,449);
		rectangle(329+i,29,479,449);
	}
	setcolor(YELLOW);
	settextstyle(DEFAULT_FONT,HORIZ_DIR,10);
	switch(floor)
	{
		case 1: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"1");
			break;
		case 2: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"2");
			break;
		case 3: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"3");
			break;
		case 4: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"4");
			break;
		case 5: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"5");
			break;
		case 6: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"6");
			break;
		case 7: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"7");
			break;
		case 8: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"8");
			break;
		case 9: outtextxy((getmaxx()/2)-(28),getmaxy()/2-35,"9");
			break;
		case 10:outtextxy((getmaxx()/2)-(55),getmaxy()/2-35,"10");
			break;
	}
	delay(500);
	cleardevice();
	closegraph();
}

void operate(lift *t,node *o,int uod,FILE *record)
{
	int to,n,i;
	fflush(stdin);
	printtime(record);
	fprintf(record,"Floors visited:");
	printf("Press any key in between to call lift\nPress any key to start.");
	getch();
	if(uod==1)
	{
		while(o->next!=NULL)
		{
			displaylift(o->tofloor);
			fprintf(record," %d ",o->tofloor);
			o=o->next;
			if(kbhit())
			{
				fflush(stdin);
				if(isliftfull(t)==TRUE)
					printf("Lift is currently full.Please try after some time.\n");
				else
				{
					do
					{
						printf("How many people are there:");
						scanf("%d",&n);
						if(n+t->count>capacity-1)
						{
							printf("MAX Lift capacity=5 persons.Please try again!\n");
							fflush(stdin);
						}
					}while(n+t->count>capacity-1);
					for(i=1;i<=n;i++)
					{
						fflush(stdin);
						printf("Which floor do you want to go:");
						scanf("%d",&to);
						insert(t,to,o->tofloor);
					}
				}
			}
			delete1(t,o->prev);
			delay(1000);
		}
		while(o!=NULL)
		{
			displaylift(o->tofloor);
			fprintf(record," %d ",o->tofloor);
			o=o->prev;
			if(kbhit())
			{
				fflush(stdin);
				if(isliftfull(t)==TRUE)
					printf("Lift is currently full.Please try after some time.\n");
				else
				{
					do
					{
						printf("How many people are there:");
						scanf("%d",&n);
						if(n+t->count>capacity-1)
						{
							printf("MAX Lift capacity=5 persons.Please try again!\n");
							fflush(stdin);
						}
					}while(n+t->count>capacity-1);
					for(i=1;i<=n;i++)
					{
						fflush(stdin);
						printf("Which floor do you want to go:");
						scanf("%d",&to);
						insert(t,to,o->tofloor);
					}
				}
			}
			delete1(t,o->next);
			delay(1000);
		}

	}
	else
	{
		while(o->prev!=NULL)
		{
			displaylift(o->tofloor);
			fprintf(record," %d ",o->tofloor);
			o=o->prev;
			if(kbhit())
			{
				fflush(stdin);
				if(isliftfull(t)==TRUE)
					printf("Lift is currently full.Please try after some time.\n");
				else
				{
					do
					{
						printf("How many people are there:");
						scanf("%d",&n);
						if(n+t->count>capacity-1)
						{
							printf("MAX Lift capacity=5 persons.Please try again!\n");
							fflush(stdin);
						}
					}while(n+t->count>capacity-1);
					for(i=1;i<=n;i++)
					{
						fflush(stdin);
						printf("Which floor do you want to go:");
						scanf("%d",&to);
						insert(t,to,o->tofloor);
					}
				}
			}
			delete1(t,o->next);
			delay(1000);
		}
		while(o!=NULL)
		{
			displaylift(o->tofloor);
			fprintf(record," %d ",o->tofloor);
			o=o->next;
			if(kbhit())
			{
				fflush(stdin);
				if(isliftfull(t)==TRUE)
					printf("Lift is currently full.Please try after some time.\n");
				else
				{
					do
					{
						printf("How many people are there:");
						scanf("%d",&n);
						if(n+t->count>capacity-1)
						{
							printf("MAX Lift capacity=5 persons.Please try again!\n");
							fflush(stdin);
						}
					}while(n+t->count>capacity-1);
					for(i=1;i<=n;i++)
					{
						fflush(stdin);
						printf("Which floor do you want to go:");
						scanf("%d",&to);
						insert(t,to,o->tofloor);
					}
				}
			}
			delete1(t,o->prev);
			delay(1000);
		}
	}
}

void readlog()
{
	char c;
	FILE *read;
	read=fopen("RECORD.TXT","r");
	clrscr();
	if(read==NULL)
	{
		printf("Record file not found.Please run the simulation to generate new record file.\n");
		getch();
		clrscr();
		return;
	}
	do
	{
		c=fgetc(read);
		delay(5);
		printf("%c",c);
	}while(c!=EOF);
	fclose(read);
	getch();
	clrscr();
	return;
}

void simulate()
{
	lift x;
	int n,ch,uod,i,to,on;
	node *o;
	FILE *record;
	x.count=0;
	x.start=x.end=NULL;
	while(1)
	{
		fflush(stdin);
		fflush(stdout);
		o=NULL;
		printf("Enter choice:  1) Start/Continue Simulation  2)View Lift logs  3) Exit:");
		scanf("%d",&ch);
		if(ch==3)
			return;
		switch(ch)
		{
			case 2: readlog();
				break;
			case 1: record=fopen("record.txt","a");
				do
				{
					printf("Which floor are you on:");
					scanf("%d",&on);
					if(on<1||on>maxfloor)
						{
							printf("Please try again!\n");
							fflush(stdin);
						}
				}while(on<1||on>maxfloor);
				printf("Select:  1)UP  2)DOWN\nYour choice:");
				scanf("%d",&uod);
				do
				{
					printf("How many people are there:");
					scanf("%d",&n);
					if(n>capacity-1)
					{
						printf("MAX Lift capacity=5 people.Please try again!\n");
						fflush(stdin);
					}
				}while(n>capacity-1);
				for(i=1;i<=n;i++)
				{
					do
					{
						printf("Which floor do you want to go:");
						scanf("%d",&to);
						if(to<1||to>maxfloor||to==on)
						{
							printf("Please try again!\n");
							fflush(stdin);
						}
					}while(to<1||to>maxfloor);
					insert(&x,to,on);
				}
				if(uod==1)
					o=searchup(&x,on);
				else
					o=searchdown(&x,on);
				operate(&x,o,uod,record);
				fprintf(record,"\n------------------------------------\n");
				printf("Simulation complete.Press any key to continue.\n");
				getch();
				fclose(record);
				clrscr();
				break;
			default: printf("Invalid choice.\n");
		}
	}
}

void welcome()
{
	int gd=DETECT,gm;
	int i=0,x=1;
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	while(!kbhit())
	{
		cleardevice();
		setcolor(15-i);
		rectangle(15,170,getmaxx()-15,getmaxy()-170);
		setcolor(i);
		settextstyle(SMALL_FONT,HORIZ_DIR,10);
		outtextxy((getmaxx()/2)-(292+x),getmaxy()/2-36,"WELCOME TO LIFT SIMULATOR");
		outtextxy((getmaxx()/2)-(291),getmaxy()/2-35,"WELCOME TO LIFT SIMULATOR");
		outtextxy((getmaxx()/2)-(290-x),getmaxy()/2-34,"WELCOME TO LIFT SIMULATOR");
		i=(i+1);
		x*=-1;
		setcolor(WHITE);
		settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
		outtextxy((getmaxx()/2)-194,getmaxy()/2+20,"press any key to continue");
		delay(150);
	}
	cleardevice();
	closegraph();
}

void endscreen()
{
	int gd=DETECT,gm;
	initgraph(&gd,&gm,"C:\\TURBOC3\\BGI");
	settextstyle(DEFAULT_FONT,HORIZ_DIR,1);
	setcolor(CYAN);
	outtextxy((getmaxx()/2)-100,getmaxy()/2-200,"DATA STRUCTURES MINI PROJECT");
	outtextxy((getmaxx()/2)-130,getmaxy()/2-150,"RINKESH PALTIWALE/ENIGMA-16102B0018");
	outtextxy((getmaxx()/2)-115,getmaxy()/2-130,"ADITYA VANDKAR/ALPHA-16102B0001");
	outtextxy((getmaxx()/2)-80,getmaxy()/2-110,"MAYUR WANVE-16102B0009");
	outtextxy((getmaxx()/2)-40,getmaxy()/2-40,"SE CMPN B");
	outtextxy((getmaxx()/2)-140,getmaxy()/2+20,"VIDYALANKAR INSTITUTE OF TECHNOLOGY");
	delay(8000);
	cleardevice();
	settextstyle(DEFAULT_FONT,HORIZ_DIR,2);
	outtextxy((getmaxx()/2)-230,getmaxy()/2-110,"We encourage all to use stairs");
	delay(3000);
	cleardevice();
	outtextxy((getmaxx()/2)-210,getmaxy()/2-110,"Hope it lifted your spirits!");
	delay(3000);
	closegraph();
}

void main()
{
	clrscr();
	welcome();
	simulate();
	endscreen();
}
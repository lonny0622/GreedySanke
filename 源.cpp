#include<graphics.h>
#include<iostream>
#include<stdlib.h>
#include<cstdlib>
#include<time.h>
#include <fstream>
using namespace std;


#define MAP_W 800
#define MAP_H 600//确定地图大小
int score ;	//个数、分数
char a[10];	//存放分数的字符型数组
char b[10];//存放时间的字符型数组
char key;			//方向

void over();//结束游戏
void Score();//统计分数
void begin();//开始界面
void goon();//继续游戏
void playhigh();//s输出最高分数
void background();//背景界面

class Food;//食物类
class Snake;//蛇类
typedef struct Position {
	int x;
	int y;
	struct Position* next;
}Position; //位置结构体 
class Snake {
public:
	Position* head, * end;//蛇头蛇尾所在坐标
	Position* p=new Position;//辅助坐标 
	int t1;
	Snake();//构造函数
	int hitwall();//判断是否撞墙
	int hitself();//判断是否撞自己
	int move(Food *f);//移动蛇
	void play(Food* f);//开始游戏
	void Tomove(Food* f);//控制游戏
	void puttime();//输出开始了多少时间
	~Snake() {
		if (head == NULL || head->next == NULL)
		{
			return;
		}

		while ((p = head) != NULL)
		{
			head = head->next;
			delete p;//释放指针空间
		}
	}//析构函数
};//蛇类 
class Food {
public:
	int xfood;
	int yfood;
	Food(Snake *a);
}; //食物类


Snake::Snake() {
	
	int i = 1;
	t1 = fclock();//记录游戏开始的时间
	end = new Position;
	end->x = 185;
	end->y = 185;
	end->next = NULL;//初始化蛇尾位置

	//创建三个蛇身位置
	for (i = 1; i <= 9; i++)
	{

		head = new Position;
		head->x = 185 + 5 * i;    				 //每个蛇身 x相差 10 
		head->y = 185;

		head->next = end;  //让新建立的头跟尾连起来 
		end = head;	//把头赋给尾，成为尾的一部分
			
	}//接下来继续建立头（循环） ，如果不建立的话，那么头就是第一个 

	while (end->next != NULL)			//这是一个逆序的过程
	{
		ege_enable_aa(true);				//抗锯齿
		PIMAGE body = newimage();
		getimage(body, "body.png");
		putimage(end->x-5, end->y-5, body);//放置身体图片
		end = end->next;//指针移到蛇的下一部分
	}
}
int Snake::hitwall(){
	if (head->x <= 45 || head->x >= 757 || head->y <= 45|| head->y >= 573) 
	{
		over();
		return 1;
	}
	return 0;
}//判断是否撞墙
int Snake::hitself() {
	p = head;
	while (p->next != NULL)
	{
		
		p = p->next;

		if (head->x == p->x && head->y == p->y)	//判断蛇头是否与其他蛇身重合
		{
			over();
			return 1;
		}

	}
	return 0;
}//判断是否撞自己
int Snake::move(Food * f) {//移动蛇的位置
	int ret;
	if ((key != 'A') && (key != 'S') && (key != 'D') && (key != 'W'))//如果不是按下控制方向： a,s,d,w ，那么就重新按 
	{
		return 0;
	}

	ret = hitwall();	
	if (ret == 1)
	{
		return 1;
	}//是否撞墙 
	ret = hitself();
	if (ret == 1)
	{
		return 1;
	}//是否咬到自己

	p = new Position;
	p->next = head;						//连接到头部 

	switch (key)
	{
	case 'D'://向右移动
		p->x = head->x + 5;//向右移动10个像素
		p->y = head->y;
		break;

	case 'W'://向上移动
		p->x = head->x;
		p->y = head->y - 5;//向上移动10个像素
		break;

	case 'S'://向下移动 
		p->x = head->x;
		p->y = head->y + 5;//向下移动10个像素 
		break;

	case 'A'://向左移动
		p->x = head->x - 5;//向左 移动10个像素
		p->y = head->y;
		break;
	}
	ege_enable_aa(true);				//抗锯齿
	PIMAGE body = newimage();
	getimage(body, "body.png");
	putimage(p->x-5, p->y-5, body);//刷新头部
	head = p;

	delay_fps(20);				//刷新

	if ((p->x <= f->xfood + 5 && p->x >= f->xfood - 5) && (p->y <= f->yfood + 5 && p->y >= f->yfood - 5))
	{
		//如果移动的一格刚好是食物的位置
		score += 1;
		delete f;
		Food *f=new Food(this);//生成新的食物
		Tomove(f);//将新的食物更新到Tomove

	}

	else	//若移动的一格刚好是食物的位置,新增的称为蛇头，不用去掉蛇尾
	{

		while (p->next->next != NULL) p = p->next;//指向蛇尾前一格，因为需要释放蛇尾，节约内存

		setfillcolor(EGERGB(0, 0, 0));
		bar(p->x - 5, p->y - 5, p->x + 5, p->y + 5);

		free(p->next);				//释放蛇尾
		p->next = NULL;
		p = head;				//将p指向head
	}
	return 0;
}
void Snake::play(Food *f) {
	key = 'D';	//刚开始，贪吃蛇默认向右移动
	Tomove(f);  
}
void Snake::Tomove(Food* f) {
	int my_ret = 0;//初始化
	char keys[4] = { 'A', 'W', 'D', 'S' };
	char key1[4] = { 'D', 'S', 'A', 'W' };
	char directkeys[4] = { key_left, key_up, key_right, key_down };
	while (1)
	{
		for (int i = 0; i < 4; i++)
			//如果蛇不是向下前进的时候，按上键，执行向上前进操作
		{
			if ((keystate(keys[i]) || keystate(directkeys[i])) && key != key1[i])
				//判断是否是某个按键按下
			{
				key = keys[i];
			}
		}
		puttime();//输出开始了多长时间
		my_ret = move(f);		//移动蛇身

		if (my_ret == 1)
		{
			break;
		}
		
	}
}
void Snake::puttime() {
	int t2 = fclock();
	setfillcolor(EGERGB(0, 0, 0));
	bar(111, 9, 177, 30);
	_itoa_s(t2 - t1, b, 10);
	setfont(25, 0, "宋体");
	outtextxy(111, 9, b);
}
Food::Food(Snake *a) {
	srand(time(0));
	
flag:

	while (1)
	{
		xfood = rand() % (750 + 1 - 50) + 50;	//随机rand()%100  ~~  [0~99]
		yfood = rand() % (565 + 1 - 50) +50;//rand产生范围数公式rand()%(m+1-n)+n;有效范围在 [n,m]

		if (xfood % 10 == 5)
		{
			break;
		}

		if (yfood % 10 == 5)
		{
			break;
		}
	}

	a->p = a->head;
	while (1)
	{

		
		if ((a->p->x <= xfood + 5 && a->p->x >= xfood - 5) && (a->p->y <= yfood + 5 && a->p->y >= yfood - 5))
			//若生成坐标和蛇重叠了，回到生成坐标循环
		{
			a->p = NULL;
			goto flag;//重新开始
		}

	if (a->p->next == NULL) //与每一段蛇身比较完毕，跳出循环 
		{
			break;
		}
		a->p = a->p->next;

	}

	//画食物
	ege_enable_aa(true);				//抗锯齿
	PIMAGE food = newimage();
	getimage(food, "food.png");
	putimage(xfood-5, yfood-5, food);
	Score();//输出分数
}



void Score()		//得分 
{
	setfillcolor(EGERGB(0, 0, 0));
	bar(310, 9, 385, 30);
	_itoa_s(score, a, 10);
	setfont(25, 0, "宋体");
	outtextxy(310, 9, a);
}
void over()			//结束界面 
{
	int x, y;
	cleardevice();
	PIMAGE c1 = newimage();
	getimage(c1, "c1.png");
	PIMAGE c2 = newimage();
	getimage(c2, "c2.png");
	for (; ; delay_fps(120))
	{
		mousepos(&x, &y);
		if (x < 500 && x>300 && y < 520 && y>450)
		{
			putimage(0, 0, c2);
			setfont(100, 0, "华文彩云");
			outtextxy(100, 150, "GAME OVER!");
			setfont(50, 0, "宋体");
			outtextxy(200, 300, "最终的得分为：");
			outtextxy(550, 300, a);
			if (keystate(VK_LBUTTON))
			{
				delimage(c2);
				break;
			}
		}
		else
		{
			putimage(0, 0,c1);
			setfont(100, 0, "华文彩云");
			outtextxy(100, 150, "GAME OVER!");
			setfont(50, 0, "宋体");
			outtextxy(200, 300, "最终的得分为：");
			outtextxy(550, 300, a);
		}
	}
	
	int highest=0;
	fstream  afile;
	afile.open("high.txt", ios::in);//打开文件
	afile >> highest;//读取最高分
	afile.close();//关
	if (score > highest)
	{
		highest = score;
		fstream  bfile;
		bfile.open("high.txt", ios::out);
		bfile << score;//写入最高分
		bfile.close();//关
	}
}
void goon() {
	int x, y;
	cleardevice();
	PIMAGE go = newimage();
	getimage(go, "go.jpg");
	PIMAGE goA = newimage();
	getimage(goA, "goA.jpg");
	PIMAGE goB = newimage();
	getimage(goB, "goB.jpg");
	for (; ; delay_fps(120))
	{
		mousepos(&x, &y);
		if (x < 510 && x>290 && y < 325 && y>235)
		{
			putimage(0, 0, goA);
			if (keystate(VK_LBUTTON))
			{
				delimage(goA);
				break;//退出循环，继续游戏
			}
		}
		else if (x < 510 && x>290 && y < 460 && y>360)
		{
			putimage(0, 0, goB);
			if (keystate(VK_LBUTTON))
			{
				exit(1);//退出游戏
			}
		}
		else
		{
			putimage(0, 0, go);
		}
	}
}
void begin()                  //开始界面
{
	score = 0;//初始化分数
	int x, y;
	cleardevice();
	PIMAGE log = newimage();
	getimage(log, "begin.jpg");
	PIMAGE logA = newimage();
	getimage(logA, "2.jpg");
	PIMAGE logB = newimage();
	getimage(logB, "3.jpg");
	PIMAGE logC = newimage();
	getimage(logC, "4.jpg");
	delay(50);
	for (; ; delay_fps(120))
	{
		mousepos(&x, &y);
		 if (x < 470 && x>300 && y < 310 && y>230)
		{
			putimage(0, 0, logA);
			if (keystate(VK_LBUTTON))
			{
				delimage(logA);
				break;
			}
		}
		else if (x < 470 && x>300 && y < 400 && y>330)
		{
			putimage(0, 0, logB);
			if (keystate(VK_LBUTTON))
			{
				exit(1);
			}
		}
		else if (x < 470 && x>300 && y < 500 && y>440)
		{
			putimage(0, 0, logC);
			if (keystate(VK_LBUTTON))
			{
				playhigh();//输出最高分数
			}
		}
		else
		{
			putimage(0, 0, log);
		}
	}

}
void playhigh() {
	char highest[10];
	fstream  afile;
	afile.open("high.txt", ios::in);//打开文件
	afile >> highest;//读取最高分
	afile.close();//关
	cleardevice();
	int x, y;
	cleardevice();
	PIMAGE c1 = newimage();
	getimage(c1, "c1.jpg");
	PIMAGE c2 = newimage();
	getimage(c2, "c2.jpg");
	for (; ; delay_fps(120))
	{
		mousepos(&x, &y);
		if (x < 500 && x>300 && y < 570 && y>500)
		{
			putimage(0, 0, c2);
			setfont(50, 0, "宋体");
			outtextxy(200, 300, "最高的得分为：");
			outtextxy(550, 300, highest);
			if (keystate(VK_LBUTTON))
			{
				delimage(c2);
				break;
			}
		}
		else
		{
			putimage(0, 0, c1);
			setfont(50, 0, "宋体");
			outtextxy(200, 300, "最高的得分为：");
			outtextxy(550, 300, highest);
		}
	}
	

}
void background()       		//背景
{
	
	setfillcolor(WHITE);
	floodfill(29, 29, WHITE);
				//抗锯齿
	PIMAGE back = newimage();
	getimage(back, "back.png");
	putimage(0,0, back);
}


int main()
{
	initgraph(MAP_W, MAP_H, 0);
	setcolor(WHITE);
	setbkcolor(EGERGB(0, 0, 0));
	setbkmode(TRANSPARENT);
	
	while (1) {

		begin();
		cleardevice();
		background();
		Snake* snake = new Snake;
		Food* f = new Food(snake);
		snake->play(f);
		delete snake;
		goon();
	}
	closegraph();
	return 0;
}

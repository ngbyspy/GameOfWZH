/*
*
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*		用VS打开该代码一定要修改属性！！！
*
* 							↗链接器→系统→子系统→窗口 (/SUBSYSTEM:WINDOWS)
*	修改方式：右键项目→配置属性→高级→字符集→使用多字节字符集
*							↘C/C++→SDL检查→否
*/
/*
	类天天酷跑项目日志
	1.创建项目
	2.安装easyX图形库//已安装过
	3.创建游戏窗口
	4.加载图片资源与音乐资源
	5.实现角色左右移动
	6.实现双击加速移动//work真的太复杂了，搞了好久
*/
#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include"Check.h"
#include"checkfiles.h"
#include<conio.h>
#include<vector>
#include<Windows.h>
/*以下头文件为时间函数所需*/
#include <sys/timeb.h>
#include <cstdio>
#include <time.h>
#include <string>
/*以上头文件为时间函数所需*/

using namespace std;//申请命名空间	

#define WIN_WIDTH 600
#define WIN_HEIGHT 700
#define OBSTACLE_COUNT 10
#define RandomNumbersMax 101

IMAGE imgBackGs[4];
IMAGE imgHeros;//储存角色奔跑图片
int heroX, heroY, heroHuY0, heroY0;//角色坐标
bool heroLeft;//角色往左移动
bool heroRight;//角色往右移动

bool ifupdate;//是否需要立即刷新页面
bool ifPause;//是否需要暂停
bool keyPProcessed;  // 新增变量，判断P键是否已经处理过
bool keyDProcessed = false;
bool keyAProcessed = false;
bool ifSpeeding = false; // 控制是否可以触发加速

int random[RandomNumbersMax];//随机数集
int randomIndex;

int HP;
int SCORE;
int maxScore;
int level;
bool ifAdded;
IMAGE imgFailures[100];
int EnemyFre0 = 50;
int EnemyFre1 = 51;
int ReadMax;
int dist;

typedef enum
{
	ENERMY1,//上升的武器
	ENERMY2,//下降的武器
	HT1,//胡桃1
	XH,//星晖，这是回血装置
	OBSTACLE_TYPE_COUNT //这是一种边界判断，放在最后一个，它对应的数刚好是障碍物的类型个数
}obstacle_type;

//存放所有障碍物的图片
vector<vector<IMAGE>> obstacleImgs; //相当于C语言的 IMAGE obstacleImgs[][];优点是行列可变

typedef struct obstacle
{
	obstacle_type type;//障碍物的类型，几号障碍物
	int imgIndex;//当前显示的图片序号
	int x, y;//障碍物的坐标
	int speed;//障碍物的速度
	int power;//碰一下障碍物会造成多少伤害
	int score;
	bool ifExist;//是否存在
	bool ifhited;//是都碰撞
	bool ifpassed;//是否通过
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];

clock_t lastDPressTime = 0; // 用于记录上一次按下D键的时间
clock_t lastAPressTime = 0; // 用于记录上一次按下D键的时间
const clock_t doublePressInterval = CLOCKS_PER_SEC / 3; // 设定双击间隔时间，例如300ms
//以上是定义部分-----------------------------------------------------------------------------------------------------------------------------------
//以下是逻辑部分-----------------------------------------------------------------------------------------------------------------------------------

//时间函数，获取时间的最后两位，毫秒级，大大增加了随机性
//int time_interval()
//{
//	struct timeb tv {};
//	struct tm* t;
//
//	ftime(&tv);
//
//	time_t now = tv.time;
//	t = localtime(&now);
//	if (t == nullptr) {
//		perror("localtime");
//		return -1;
//	}
//
//	//printf("time_now: %d-%d-%d %d:%d:%d.%d\n", 1900+t->tm_year, 1+t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, tv.millitm);
//	return tv.millitm % 100;
//}

//生成障碍物，写这个函数的时候对整个代码都已经比较熟悉了，没怎么写注释
//void createObstacle()
//{
//	int i;
//	for (i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist == false)
//		{
//			break;
//		}
//	}
//	if (i >= OBSTACLE_COUNT) return;
//
//	obstacles[i].ifExist = true;
//	obstacles[i].ifhited = false;
//	obstacles[i].ifpassed = false;
//	obstacles[i].imgIndex = 0;
//	if (randomIndex >= RandomNumbersMax) randomIndex = 0;
//	random[randomIndex] = time_interval();
//	obstacles[i].type = (obstacle_type)(random[randomIndex++] % (OBSTACLE_TYPE_COUNT - 0));//把多个柱子放到一起，不然柱子的出现概率会过大
//	obstacles[i].x = WIN_WIDTH;
//	obstacles[i].y = 360 - obstacleImgs[obstacles[i].type][0].getheight();
//	if (obstacles[i].type == TORTOISE)
//	{
//		obstacles[i].speed = 1;
//		obstacles[i].power = 5 + level * 5;
//		obstacles[i].score = 5;
//	}
//	else if (obstacles[i].type == LION)
//	{
//		obstacles[i].speed = 4;
//		obstacles[i].power = 15 + level * 5;
//		obstacles[i].score = 11;
//	}
//	else if (obstacles[i].type == ROLLINGTORT)
//	{
//		obstacles[i].speed = 2;
//		obstacles[i].power = 6 + level * 5;
//		obstacles[i].score = 6;
//	}
//	else if (obstacles[i].type >= HOOK1 && obstacles[i].type <= HOOK2)
//	{
//		obstacles[i].speed = 0;
//		obstacles[i].power = 21 + level * 5;
//		obstacles[i].y = 0;
//		obstacles[i].score = 12;
//	}
//}

//碰撞检测
//void checkHit()
//{
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist && !obstacles[i].ifhited)
//		{
//			MyTriangle HeroLegs;
//			MyRectangle HeroBody, ObstacleBody;
//			int a1x, a1y, a2x, a2y;//角色的身体矩形坐标信息
//			if (!ifHeroSquating && !isHeroJumping)//奔跑的检测
//			{
//				HeroBody.x = heroX + 119;
//				HeroBody.y = heroY + 38;
//				a2x = heroX + imgHeros[i].getwidth() - 50;
//				a2y = heroY + imgHeros[i].getheight() - 75;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//				//以上是身体部分的矩形计算
//				//由于三角的坐标可以直接使用heroX和heroY的相对位置，我们直接用就好
//
//				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
//				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
//				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
//				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
//				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
//				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];
//
//			}
//			if ((isHeroJumping || !isHeroGrounded) && !ifHeroSquating)//在跳
//			{
//				HeroBody.x = heroX + 91;
//				HeroBody.y = heroY + 26;
//				a2x = heroX + imgHeros[i].getwidth() - 41;
//				a2y = heroY + imgHeros[i].getheight() - 74;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//
//				HeroLegs.x1 = heroX + heroLegsL[heroIndex][0];
//				HeroLegs.y1 = heroY + heroLegsL[heroIndex][1];
//				HeroLegs.x2 = heroX + heroLegsL[heroIndex][2];
//				HeroLegs.y2 = heroY + heroLegsL[heroIndex][3];
//				HeroLegs.x3 = heroX + heroLegsL[heroIndex][4];
//				HeroLegs.y3 = heroY + heroLegsL[heroIndex][5];
//			}
//			if (ifHeroSquating)//按了S键
//			{
//				HeroBody.x = squatX + 5;
//				HeroBody.y = squatY + 3;
//				a2x = squatX + imgHeroSquat[squatImgNumbers - 1].getwidth() - 10;
//				a2y = squatY + imgHeroSquat[squatImgNumbers - 1].getheight() - 6;
//				HeroBody.width = a2x - HeroBody.x;
//				HeroBody.height = a2y - HeroBody.y;
//			}
//			//障碍物的矩形坐标信息
//			IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];
//			int b1x = obstacles[i].x + 5;
//			int b1y = obstacles[i].y + 5;
//			int b2x = obstacles[i].x + img.getwidth() - 5;
//			int b2y = obstacles[i].y + img.getheight();
//			ObstacleBody.x = b1x;
//			ObstacleBody.y = b1y;
//			ObstacleBody.width = b2x - b1x;
//			ObstacleBody.height = b2y - b1y;
//
//			if (checkRectangleCollision(HeroBody, ObstacleBody))//记得添加“或者腿部碰撞”
//			{
//				HP -= obstacles[i].power;
//				obstacles[i].ifhited = true;
//				if (HP > 0)
//				{
//					mciSendString("play res/hit.mp3", 0, 0, 0);
//				}
//			}
//			if (!ifHeroSquating && checkTriangleRectangleCollision(HeroLegs, ObstacleBody))
//			{
//				HP -= obstacles[i].power;
//				obstacles[i].ifhited = true;
//				if (HP > 0)
//				{
//					mciSendString("play res/hit.mp3", 0, 0, 0);
//				}
//			}
//		}
//		/*if (obstacles[i].ifExist && obstacles[i].ifhited)
//		{
//			putimagePNG2()
//		}*/
//	}
//}

//加分检测
//void checkScore()
//{
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist && !obstacles[i].ifpassed && !obstacles[i].ifhited)
//		{
//			if (obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < heroX)
//			{
//				SCORE += obstacles[i].score;
//				maxScore += obstacles[i].score;
//				obstacles[i].ifpassed = true;
//				//printf("%d\n", SCORE);
//			}
//		}
//		if (SCORE >= 100)
//		{
//			SCORE = 0;
//			level++;
//			ifAdded = false;
//		}
//		if (level >= 0 && !ifAdded)
//		{
//			for (int j = 0; j < 3; j++)
//			{
//				EnemyFre0 -= (level * 3);
//				if (EnemyFre0 <= 25) EnemyFre0 = 25;
//				EnemyFre1 -= (level * 2);
//				if (EnemyFre1 < 30) EnemyFre1 = 30;
//				bgflySpeed[j] += level;
//			}
//			ifAdded = true;
//		}
//	}
//}

//渲染游戏背景
void updateBackGroud()
{
	putimagePNG2(0, 0, WIN_WIDTH, &imgBackGs[0]);
}

//检测玩家是否需要中途退出
void ifWannaExit()
{
	if (HP > 0)
	{
		if (IDYES == MessageBoxA(nullptr, "游戏未保存，是否退出？", "注意", MB_YESNO))
		{
			exit(0);
		}
	}
	else
	{
		if (IDYES == MessageBoxA(nullptr, "数据已保存，是否立即退出？", "DailyRunningNG", MB_YESNO))
		{
			exit(0);
		}
	}
}

void speedUpHero() 
{
	if (ifSpeeding) 
	{
		dist *= 2; // Double the speed
	}
	else dist = 5;
}

//接收输入
void keyEvent()
{
	//玩家一的按键事件
	bool keyAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;  // 玩家一左移
	bool keyDPressed = (GetAsyncKeyState('D') & 0x8000) != 0;  // 玩家一右移

	//其他按键
	bool keyEscPressed = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;//Esc退出键
	bool keyPPressed = (GetAsyncKeyState('P') & 0x8000) != 0;

	if (keyEscPressed)
	{
		ifWannaExit();
	}

	//下面是检测P键的按下，由于GetAsyncKeyState函数仅检测单次按键状态，所以按下和弹起都是true，所以要加一个keyProcessed来判断按下和弹起
	if (keyPPressed && !keyPProcessed)
	{
		// 当P键被按下且之前未处理时，切换暂停状态
		ifPause = !ifPause;
		keyPProcessed = true;  // 标记为已处理
	}
	else if (!keyPPressed && keyPProcessed)
	{
		// 当P键被释放且之前已处理时，重置处理标志
		keyPProcessed = false;
	}

	//A和D，理由与P一样
	if (keyAPressed)
	{
		heroLeft = true;
	}
	if (keyDPressed)
	{
		heroRight = true;
	}

	// 处理D键
	if (keyDPressed && !keyDProcessed)
	{
		clock_t currentTime = clock();
		clock_t interval = currentTime - lastDPressTime;

		if (lastDPressTime != 0 && interval < doublePressInterval)
		{
			ifSpeeding = true;
			lastDPressTime = 0; // 防止重复触发加速
		}
		else
		{
			lastDPressTime = currentTime;
		}
		speedUpHero();
		keyDProcessed = true;
	}
	else if (!keyDPressed && keyDProcessed)
	{
		ifSpeeding = false;
		speedUpHero();
		keyDProcessed = false;
	}

	// 处理A键
	if (keyAPressed && !keyAProcessed)
	{
		clock_t currentTime = clock();
		clock_t interval = currentTime - lastAPressTime;

		if (lastAPressTime != 0 && interval < doublePressInterval)
		{
			ifSpeeding = true;
			lastAPressTime = 0; // 防止重复触发加速
		}
		else
		{
			lastAPressTime = currentTime;
		}
		speedUpHero();
		keyAProcessed = true;
	}
	else if (!keyAPressed && keyAProcessed)
	{
		ifSpeeding = false;
		speedUpHero();
		keyAProcessed = false;
	}
}

//更新敌方
//void updateEnemy()
//{
//	//更新障碍物
//	for (int i = 0; i < OBSTACLE_COUNT; i++)
//	{
//		if (obstacles[i].ifExist)
//		{
//			putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WIDTH,
//				&obstacleImgs[obstacles[i].type][obstacles[i].imgIndex]);
//		}
//	}
//}

//更新角色
void updateHeros()
{
		putimagePNG2(heroX, heroY, &imgHeros);//输出角色的奔跑和跳跃
	//}
	//if (ifHeroSquating)//复杂的坐标计算，唉，不过蹲下就不需要计算三角形了
	//{
	//	if (heroIndex < squatImgNumbers - 6)
	//	{
	//		putimagePNG2(heroX, heroY, &imgHeros[heroIndex]);
	//		putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
	//	}
	//	else if (heroIndex < squatImgNumbers - 4)
	//	{
	//		putimagePNG2(heroX, heroY, &imgHeroSquat[heroIndex]);
	//	}
	//	else if (heroIndex < squatImgNumbers - 1)
	//	{
	//		int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[squatImgNumbers - 1].getwidth() * 0.5;
	//		int xChannel = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
	//		int yChannel = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		putimagePNG2(x, y, &imgHeroSquat[squatImgNumbers - 1]);
	//		putimagePNG2(xChannel, yChannel, &imgHeroSquat[heroIndex]);
	//	}
	//	else
	//	{
	//		int y = heroY + imgHeros[heroIndex].getheight() - imgHeroSquat[heroIndex].getheight();
	//		int x = heroX + imgHeros[0].getwidth() * 0.5 - imgHeroSquat[heroIndex].getwidth() * 0.5;
	//		putimagePNG2(x, y, &imgHeroSquat[heroIndex]);
	//		squatX = x;
	//		squatY = y;
	//	}
	//}
}

//更新血条和分数条
//void updateBloodBar()
//{
//	drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED, HP / 100.0);//血条
//	drawBloodBar(10, 30, 200, 10, 2, BLUE, DARKGRAY, YELLOW, SCORE / 100.0);//分数条
//
//	//更新分数显示
//	setbkmode(TRANSPARENT);
//	settextcolor(BLACK);
//
//	//当前总分
//	TCHAR strPresentScore[] = _T("当前总分：");
//	outtextxy(WIN_WIDTH + 10, 120, strPresentScore);
//	TCHAR strScore[500];
//	_stprintf(strScore, _T("%d"), maxScore);
//	outtextxy(WIN_WIDTH + 90, 121, strScore);
//
//	//历史最高分
//	TCHAR strHistoryMaxScore[] = _T("历史最高分：");
//	outtextxy(WIN_WIDTH + 10, 139, strHistoryMaxScore);
//	TCHAR strScoreRead[500];
//	_stprintf(strScoreRead, _T("%d"), ReadMax);
//	outtextxy(WIN_WIDTH + 102, 140, strScoreRead);
//
//}

//死亡检测
//void checkHP()
//{
//	if (HP <= 0)
//	{
//		int grade = (maxScore >= ReadMax ? maxScore : ReadMax);
//		WriteGrade(grade);
//		mciSendString("stop res/traveling.mp3", 0, 0, 0);
//
//		char name[64] = { 0 };
//		int i = 0, j = 0;
//		int timers = 0;
//		int Tmax = 15;
//		mciSendString("play res/failure.mp3", 0, 0, 0);
//		while (i < 5446)
//		{
//			keyEvent();//接收输入函数
//			timers += getDelay();//加上底层逻辑基础单位时间
//			if (timers >= Tmax)
//			{
//				timers = 0;
//				sprintf(name, "res/failure/%04d.jpg", i + 1);
//				j = i % 10;//j从0到9
//				loadimage(&imgFailures[j], name);
//				putimage(0, 0, &imgFailures[j]);
//				i++;
//			}
//		}
//
//		mciSendString("stop res/failure.mp3", 0, 0, 0);
//		//putimage(0, 0, &imgFailures[0]);
//		ifWannaExit();
//	}
//}

//计算下一帧图片的数据
void fly()
{
	//碰撞检测
	//checkHit();

	//角色左右变化
	if (heroLeft && !heroRight && heroX > 0)
	{
		heroX -= dist;
		if (heroX < 0)
		{
			heroX += dist;
		}
		heroLeft = false;
	}
	if (!heroLeft && heroRight)
	{
		heroX += dist;
		if (heroX >= WIN_WIDTH - imgHeros.getwidth())
		{
			heroX = WIN_WIDTH - imgHeros.getwidth();
		}
		heroRight = false;
	}
	if (heroLeft && heroRight)
	{
		heroLeft = false;
		heroRight = false;
	}
	
	//显示障碍物的变化
	//static int frameCount = 0;//定义静态变量扩大障碍物的域
	//static int EnemyFre = 70;//每隔多少帧

	//frameCount++;//数一下这是第几帧
	//if (frameCount > EnemyFre)
	//{
	//	frameCount = 0;
	//	if (randomIndex >= RandomNumbersMax) randomIndex = 0;
	//	random[randomIndex] = time_interval();
	//	EnemyFre = EnemyFre0 + random[randomIndex++] % EnemyFre1;
	//	createObstacle();
	//}
	////更新障碍物的坐标
	//for (int i = 0; i < OBSTACLE_COUNT; i++)
	//{
	//	if (obstacles[i].ifExist)
	//	{
	//		obstacles[i].x -= (bgflySpeed[2] + obstacles[i].speed);
	//		if (obstacles[i].x < -obstacleImgs[obstacles[i].type][obstacles[i].imgIndex].getwidth() * 2)
	//		{
	//			obstacles[i].ifExist = false;
	//		}
	//		int len = obstacleImgs[obstacles[i].type].size();
	//		obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;
	//	}
	//}
}

//游戏初始化
void init()
{
	MessageBoxA(0, "不可沉迷游戏！该游戏是由一个新手写的，多次运行可能会出现异常！", "忠告", MB_OK);

	//创建游戏窗口
	initgraph(WIN_WIDTH, WIN_HEIGHT, EX_DBLCLKS);

	char name[64] = { 0 };

	//暂无初始音乐
	////初始音乐
	//preLoadSound("res/beginning.mp3");
	//mciSendString("play res/beginning.mp3", 0, 0, 0);

	//暂时不检测资源
	////检查资源完整性
	char tempName[64] = { 0 };

	//mciSendString("stop res/beginning.mp3", 0, 0, 0);

	//加载背景资源
	sprintf(name, "res/bg.png");
	loadimage(&imgBackGs[0], name, WIN_WIDTH, WIN_HEIGHT);

	//加载角色
	sprintf(name, "res/zl.png");
	loadimage(&imgHeros, name);
	char relativePath[100] = ".\\res";  // 相对路径
	char filename[64] = { 0 };  // 要检查的文件名
	sprintf(filename, "zl.png");
	if (!isFileExists(relativePath, filename))
	{
		MessageBoxA(nullptr, "资源加载错误！请联系管理员检查资源完整性！错误码：0x1110Failure", "警告", MB_OK);
		exit(0);
	}

	//初始化角色位置信息
	heroX = WIN_WIDTH * 0.5 - imgHeros.getwidth() * 0.5;
	heroY = WIN_HEIGHT * 0.5 - imgHeros.getheight() * 0.5;

	ifupdate = true;//一开始是需要刷新的

	//封装加载障碍物
	////加载障碍物1-乌龟1
	//IMAGE imgTort;
	//loadimage(&imgTort, "res/t1.png");
	//vector<IMAGE> imgTortArray;
	//imgTortArray.push_back(imgTort);
	//obstacleImgs.push_back(imgTortArray);
	////加载障碍物2-狮子
	//IMAGE imgLion;
	//vector<IMAGE> imgLionArray;
	//for (int i = 0; i < 6; i++)
	//{
	//	sprintf(name, "res/p%d.png", i + 1);
	//	loadimage(&imgLion, name);
	//	imgLionArray.push_back(imgLion);
	//}
	//obstacleImgs.push_back(imgLionArray);
	////加载障碍物3-旋转的乌龟
	//IMAGE imgRollingTort;
	//vector<IMAGE> imgRollingTortArray;
	//for (int i = 0; i < 6; i++)
	//{
	//	sprintf(name, "res/t%d.png", i + 2);
	//	loadimage(&imgRollingTort, name);
	//	imgRollingTortArray.push_back(imgRollingTort);
	//}
	//obstacleImgs.push_back(imgRollingTortArray);
	////加载障碍物4-柱子
	//IMAGE imgHOOK;
	//for (int i = 0; i < 4; i++)
	//{
	//	vector<IMAGE> imgHookArray;
	//	sprintf(name, "res/h%d.png", i + 1);
	//	loadimage(&imgHOOK, name, 63, 260, true);
	//	imgHookArray.push_back(imgHOOK);
	//	obstacleImgs.push_back(imgHookArray);
	//}
	////初始化障碍物池
	//for (int i = 0; i < OBSTACLE_COUNT; i++)
	//{
	//	obstacles[i].ifExist = false;
	//	obstacles[i].ifhited = false;
	//}

	ifPause = false;
	keyPProcessed = false;

	for (int i = 0; i < 100; i++) random[i] = 0;
	randomIndex = 0;

	HP = 100;
	SCORE = 0;
	level = 0;
	maxScore = 0;
	heroLeft = false;
	heroRight = false;
	ifAdded = true;
	ReadMax = ReadGrade();
	dist = 5;

	//预加载音效
	/*preLoadSound("res/hit.mp3");
	preLoadSound("res/failure.mp3");*/
	cleardevice(); // 清空屏幕内容
	//开始游戏，其实就是loading的假图，目的是提醒玩家游戏要开始了，给个2秒左右的反应时间
	//sprintf(tempName, "res/start.png");
	//loadimage(&imgCHECKING, tempName);
	//for (int i = 0; i < 200; i++)
	//{
	//	BeginBatchDraw();//开始渲染
	//	putimagePNG2((WIN_WIDTH + 196) / 2.0 - imgCHECKING.getwidth() / 2.0, WIN_HEIGHT - 100, &imgCHECKING);
	//	drawBloodBar((WIN_WIDTH + 196) / 2.0 - 200, WIN_HEIGHT - 70, 400, 5, 2, BLACK, DARKGRAY, WHITE, 1);
	//	EndBatchDraw();//结束本次渲染
	//	Sleep(10);
	//}
	/*preLoadSound("res/traveling.mp3");
	mciSendString("play res/traveling.mp3 repeat", 0, 0, 0);*/
}

//主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow)
{
	init();//启动主窗口

	int timer = 0;//使用计时器优化帧
	while (1)//每次循环是一帧
	{
		keyEvent();//接收输入函数
		timer += getDelay();//加上底层逻辑基础单位时间
		if ((timer > 30 || ifupdate) && !ifPause)
		{
			timer = 0;
			ifupdate = false;

			BeginBatchDraw();//绘画背景
			updateBackGroud();//更新背景
			//system("pause");//用来调试
			updateHeros();
			//updateEnemy();
			//updateBloodBar();
			//printf("%d\n", heroIndex);//用来调试
			EndBatchDraw();//结束本次渲染
			//checkHP();
			//checkScore();
			fly();//坐标移动，为下一次跳跃做准备；
		}
	}
	return 0;
}